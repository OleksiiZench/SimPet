// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTService_UpdateAnimalState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Animals/SimPetAnimal.h"

UBTService_UpdateAnimalState::UBTService_UpdateAnimalState()
{
	NodeName = "Update Animal State";
}

void UBTService_UpdateAnimalState::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController *AIController = OwnerComp.GetAIOwner();
	ASimPetAnimal *Animal = AIController ? Cast<ASimPetAnimal>(AIController->GetPawn()) : nullptr;
	
	if (Animal)
	{
		uint8 EnumValue = (uint8)Animal->GetAnimalState();
		
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), EnumValue);  // Записуємо в BlackBoard
	}
}
