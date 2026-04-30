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
		int32 StateAsByte = static_cast<int32>(Animal->GetAnimalState());
		
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), StateAsByte);  // Записуємо в BlackBoard
	}
}
