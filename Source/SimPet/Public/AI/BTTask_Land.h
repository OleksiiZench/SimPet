// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_Land.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_Land : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Land();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
