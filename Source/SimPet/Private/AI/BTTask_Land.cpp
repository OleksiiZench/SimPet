// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_Land.h"
#include "AIController.h"

#include "Characters/Animals/SimPetCanary.h"

UBTTask_Land::UBTTask_Land()
{
	NodeName = "Land";
}

EBTNodeResult::Type UBTTask_Land::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	AAIController *AIController = OwnerComp.GetAIOwner();
	ASimPetCanary *Canary = AIController ? Cast<ASimPetCanary>(AIController->GetPawn()) : nullptr;
	if (Canary)
		Canary->SetFlyingMovementMode(false);
		
	return EBTNodeResult::Succeeded;
}
