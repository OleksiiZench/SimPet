// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");
	
	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, HomeLocationKey));
}

void UBTTask_FindRandomLocation::InitializeFromAsset(UBehaviorTree &Asset)
{
	Super::InitializeFromAsset(Asset);
	
	UBlackboardData *BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		HomeLocationKey.ResolveSelectedKey(*BBAsset);  // Ця функція знаходить ID ключа і записує його всередину структури HomeLocationKey
	}
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
	
	// 1. Отримуємо котролер та пішака
	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController ? AIController->GetPawn() : nullptr;
	
	if (!AIPawn)
		return EBTNodeResult::Failed;
	
	// 2. Отримуємо навігаційну систему
	UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
		return EBTNodeResult::Failed;
	
	// 3. Отримуємо домашню позицію pawn
	FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(HomeLocationKey.GetSelectedKeyID());  // Оптимізований метод який використовує ID за мість Name
	if (HomeLocation.Equals(FVector::ZeroVector))
		HomeLocation = AIPawn->GetActorLocation();
	
	// 4. Шукаємо випадкову точка на NavMesh навколо тварини
	float ActualRadius = BB->GetValueAsFloat(RadiusKey.SelectedKeyName);
	
	FNavLocation ResultLocation;
	
	bool bFound = NavSys->GetRandomPointInNavigableRadius(
		HomeLocation,
		ActualRadius,
		ResultLocation
	);
	
	// 5. Записуємо результат у ключ TargetLocation
	if (bFound)
	{
		BB->SetValueAsVector(GetSelectedBlackboardKey(), ResultLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
