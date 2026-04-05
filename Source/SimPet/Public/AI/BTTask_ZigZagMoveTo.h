// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_ZigZagMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_ZigZagMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ZigZagMoveTo();
	
	virtual void InitializeFromAsset(UBehaviorTree &Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector RadiusKey; // Радіус пошуку точки
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HomeLocationKey;  // Ключ координат дому
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptableRadius;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float OffsetDistance = 50.0f;
	
private:
	int32 CurrentPointIndex;
	TArray<FVector> Waypoints;
};
