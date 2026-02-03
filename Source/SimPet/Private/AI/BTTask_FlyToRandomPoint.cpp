// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_FlyToRandomPoint.h"
#include "AIController.h"

#include "Characters/Animals/SimPetCanary.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_FlyToRandomPoint::UBTTask_FlyToRandomPoint()
{
	FlyHeightMin = 300.0f;
	FlyHeightMax = 600.0f;
	FlyRadius = 500.0f;
	
	NodeName = "Fly To Random Point";
	
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FlyToRandomPoint::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	AAIController *AIController = OwnerComp.GetAIOwner();
	ASimPetCanary *Canary = AIController ? Cast<ASimPetCanary>(AIController->GetPawn()) : nullptr;
	
	if (!Canary)
		return EBTNodeResult::Failed;
	
	// 1. Вмикаємо режим польоту
	Canary->SetFlyingMovementMode(true);
	
	// 2. Рахуємо точку в повітрі
	FVector CurrentLocation = Canary->GetActorLocation();
	
	// Випадкова точка в колі + випадкова висота
	FVector RandomOffset = FMath::VRand();
	RandomOffset.Z = 0;  // Спочатку плоска
	RandomOffset *= FlyRadius;  // Розтягуємо
	RandomOffset.Z = FMath::RandRange(FlyHeightMin, FlyHeightMax);  // + висота
	
	FVector DestLocation = CurrentLocation + RandomOffset;
	
	// 3. Переміщаємось на точку
	AIController->MoveToLocation(DestLocation, 50.0f, false, true, false, false);

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyToRandomPoint::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaTime);
	
	AAIController *AIController = OwnerComp.GetAIOwner();
	
	if (AIController && AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
