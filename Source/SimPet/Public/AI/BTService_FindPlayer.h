// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BTService_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API UBTService_FindPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_FindPlayer();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRange;  // Дистанція виявлення
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxDistanceFromHome;  // Максимальна відстань куди можна забігати
	
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HomeLocationKey;  // Ключ координат дому
};
