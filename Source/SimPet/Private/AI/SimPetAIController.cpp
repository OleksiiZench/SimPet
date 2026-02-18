// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/SimPetAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/Animals/SimPetAnimal.h"

void ASimPetAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ASimPetAnimal *Animal = Cast<ASimPetAnimal>(InPawn);
	
	if (Animal && Animal->BehaviorTreeAsset)
	{
		// 1. Запуск дерева
		RunBehaviorTree(Animal->BehaviorTreeAsset);
		
		// 2. Запис даних в BB
		UBlackboardComponent *BB = GetBlackboardComponent();
		
		if (BB)
		{
			FVector TargetHomeLocation;
			
			if (Animal->bUseCustomHomeLocation)
			{
				TargetHomeLocation = Animal->CustomHomeLocation;
			}
			else
			{
				TargetHomeLocation = Animal->GetActorLocation();
			}
			
			BB->SetValueAsVector(TEXT("HomeLocation"), TargetHomeLocation);
		}
	}
}
