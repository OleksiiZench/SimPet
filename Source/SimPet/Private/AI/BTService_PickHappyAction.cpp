// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTService_PickHappyAction.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "SimPetDebugHelper.h"

UBTService_PickHappyAction::UBTService_PickHappyAction()
{
	NodeName = "Pick Happy Action";
	
	FlyChance = 0.3f;
	
	Interval = 2.0f;
	RandomDeviation = 0.5;
}

void UBTService_PickHappyAction::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
		return;
	
	float Roll = FMath::FRand();
	
	int32 ActionToSet = 0;  // Walk
	
	if (Roll <= FlyChance)
	{
		ActionToSet = 1;  // Fly
	}
	
	BB->SetValueAsInt(GetSelectedBlackboardKey(), ActionToSet);
}
