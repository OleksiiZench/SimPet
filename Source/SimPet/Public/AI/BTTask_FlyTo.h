// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_FlyTo.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_FlyTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyTo();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptableRadius;
};
