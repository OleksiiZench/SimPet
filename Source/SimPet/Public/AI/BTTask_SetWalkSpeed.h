// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_SetWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_SetWalkSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_SetWalkSpeed();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0"))
	float WalkSpeed;
};
