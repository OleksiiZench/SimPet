// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/SimPetAIController.h"

#include "Characters/Animals/SimPetAnimal.h"

void ASimPetAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ASimPetAnimal *Animal = Cast<ASimPetAnimal>(InPawn);
	
	if (Animal && Animal->BehaviorTreeAsset)
	{
		RunBehaviorTree(Animal->BehaviorTreeAsset);
	}
}
