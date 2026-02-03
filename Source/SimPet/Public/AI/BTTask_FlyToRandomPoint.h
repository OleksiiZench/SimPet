// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_FlyToRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTTask_FlyToRandomPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyToRandomPoint();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere)
	float FlyHeightMin;
	
	UPROPERTY(EditAnywhere)
	float FlyHeightMax;
	
	UPROPERTY(EditAnywhere)
	float FlyRadius;
};
