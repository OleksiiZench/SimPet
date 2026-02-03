// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BTService_UpdateAnimalState.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTService_UpdateAnimalState : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateAnimalState();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
