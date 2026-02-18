// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_SetWalkSpeed.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SetWalkSpeed::UBTTask_SetWalkSpeed()
{
	NodeName = "Set Walk Speed";
	
	WalkSpeed = 300.0f;
}

EBTNodeResult::Type UBTTask_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	AAIController *AIController = OwnerComp.GetAIOwner();
	ACharacter *Character = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;
	
	if (!Character)
		return EBTNodeResult::Failed;
	
	UCharacterMovementComponent *Movement = Character->GetCharacterMovement();
	
	if (Movement)
	{
		Movement->MaxWalkSpeed = WalkSpeed;
	}
	
	return EBTNodeResult::Succeeded;
}
