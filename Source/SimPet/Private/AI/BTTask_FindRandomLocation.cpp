// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	// 1. Отримуємо котролер та пішака
	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController ? AIController->GetPawn() : nullptr;
	
	if (!AIPawn)
		return EBTNodeResult::Failed;
	
	// 2. Отримуємо навігаційну систему
	UNavigationSystemV1 *NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys)
		return EBTNodeResult::Failed;
	
	FNavLocation ResultLocation;
	
	// 3. Шукаємо випадкову точка на NavMesh навколо тварини
	bool bFound = NavSys->GetRandomPointInNavigableRadius(
		AIPawn->GetActorLocation(),
		Radius,
		ResultLocation
	);
	
	// 4. Записуємо результат у ключ TargetLocation
	if (bFound)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), ResultLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
