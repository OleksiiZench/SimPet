// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_ZigZagMoveTo.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Math/UnrealMathUtility.h"

UBTTask_ZigZagMoveTo::UBTTask_ZigZagMoveTo()
{
	NodeName = TEXT("Zig Zag Move To");
	
	bCreateNodeInstance = true;
	bNotifyTick = true;
	
	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ZigZagMoveTo, HomeLocationKey));
	
	Radius = 350.0f;
}

void UBTTask_ZigZagMoveTo::InitializeFromAsset(UBehaviorTree &Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData *BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		HomeLocationKey.ResolveSelectedKey(*BBAsset);  // Ця функція знаходить ID ключа і записує його всередину структури HomeLocationKey
	}
}

EBTNodeResult::Type UBTTask_ZigZagMoveTo::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	// 1. Отримуємо котролер та Pawn
	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController ? AIController->GetPawn() : nullptr;
	
	if (!AIPawn)
		return EBTNodeResult::Failed;
	
	// 2. Отримуємо навігаційну систему
	UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
		return EBTNodeResult::Failed;
	
	// 3. Отримуємо поточну та домашню позицію позіції pawn
	FVector CurrentLocation = AIPawn->GetActorLocation();
		
	FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(HomeLocationKey.GetSelectedKeyID());  // Оптимізований метод який використовує ID за мість Name
	if (HomeLocation.Equals(FVector::ZeroVector))
		HomeLocation = CurrentLocation;
	
	// 4. Шукаємо випадкову точка на NavMesh навколо тварини
	FNavLocation ResultLocation;
	FVector TargetLocation;
	
	bool bFound = NavSys->GetRandomPointInNavigableRadius(
		HomeLocation,
		Radius,
		ResultLocation
	);
	
	if (!bFound)
	{
		return EBTNodeResult::Failed;
	}
	
	TargetLocation = ResultLocation.Location;
	
	// 5. Вираховуємо Waypoints до Targetlocation
	CurrentPointIndex = 0;
	Waypoints.Empty();
	
	float DistToTarget = FVector::Dist(CurrentLocation, TargetLocation);
	
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector LeftVector = FVector::CrossProduct(Direction, FVector::UpVector);
	
	int32 NumOfZigZags = FMath::FloorToInt(DistToTarget / 100);  // Отримуєм кількість точок поворотів
	
	float SideMultiplier = 1.0f;
	
	for (int32 i = 1; i <= NumOfZigZags; i++)
	{
		float Alpha = float(i) / (float)(NumOfZigZags + 1);
		
		FVector CenterPoint = FMath::Lerp(CurrentLocation, TargetLocation, Alpha);
		
		FVector TheoreticalWaypoint = CenterPoint + (LeftVector * OffsetDistance * SideMultiplier);
		
		FNavLocation ProjectedLocation;
		
		bool bIsOnNavMesh = NavSys->ProjectPointToNavigation(TheoreticalWaypoint, ProjectedLocation, FVector(50.0f, 50.0f, 50.0f));
		
		if (bIsOnNavMesh)
		{
			Waypoints.Add(ProjectedLocation.Location);
		}
		else
		{
			return EBTNodeResult::Failed;
		}
		
		SideMultiplier *= -1.0f;
	}
	
	Waypoints.Add(TargetLocation);
	
	// 6. Починаємо рух
	AIController->MoveToLocation(Waypoints[0], AcceptableRadius);
		
	return EBTNodeResult::InProgress;
}

void UBTTask_ZigZagMoveTo::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// 1. Отримуємо котролер та Pawn
	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController->GetPawn();
	
	float DistSq2D = FVector::DistSquared2D(AIPawn->GetActorLocation(), Waypoints[CurrentPointIndex]);
	if (DistSq2D <= FMath::Square(AcceptableRadius + 50.0f))
	{
		CurrentPointIndex++;
		
		if (CurrentPointIndex >= Waypoints.Num())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			AIController->MoveToLocation(Waypoints[CurrentPointIndex], AcceptableRadius);
		}
	}
}
