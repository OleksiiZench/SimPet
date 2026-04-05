// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_FindRandomSkyLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_FindRandomSkyLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FindRandomSkyLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector FlyRadiusKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float FlyHeightMin;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float FlyHeightMax;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HomeLocationKey;
};
