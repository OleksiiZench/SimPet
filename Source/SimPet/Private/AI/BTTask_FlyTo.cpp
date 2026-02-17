// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_FlyTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "Characters/Animals/SimPetCanary.h"

UBTTask_FlyTo::UBTTask_FlyTo()
{
	AcceptableRadius = 50.0f;
	
	NodeName = "Fly To Target";
	bNotifyTick = true;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FlyTo, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FlyTo::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	AAIController *AIController = OwnerComp.GetAIOwner();
	ASimPetCanary *Canary = AIController ? Cast<ASimPetCanary>(AIController->GetPawn()) : nullptr;
	
	if (!Canary)
		return EBTNodeResult::Failed;
	
	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());  // Отримуємо ціль з Blackboard
	
	Canary->SetFlyingMovementMode(true);  // Вмикаємо режим польоту
	
	// Запускаємо рух
	EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(
		TargetLocation,
		AcceptableRadius,
		false,
		false,
		false,
		true,
		0,
		true
	);
	
	if (Result == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_FlyTo::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	AAIController *AIController = OwnerComp.GetAIOwner();
	
	if (AIController && AIController->GetMoveStatus() == EPathFollowingStatus::Type::Idle)
	{// Перевіряємо, чи долетіли
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
