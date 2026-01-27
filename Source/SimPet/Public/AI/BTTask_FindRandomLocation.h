// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FindRandomLocation();

	virtual void InitializeFromAsset(UBehaviorTree &Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float Radius; // Радіус пошуку точки
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HomeLocationKey;  // Ключ координат дому
};
