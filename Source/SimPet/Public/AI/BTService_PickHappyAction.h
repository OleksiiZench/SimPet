// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BTService_PickHappyAction.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTService_PickHappyAction : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_PickHappyAction();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float FlyChance;
};
