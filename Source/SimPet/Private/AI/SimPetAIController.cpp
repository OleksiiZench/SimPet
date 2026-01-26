// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/SimPetAIController.h"

void ASimPetAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
