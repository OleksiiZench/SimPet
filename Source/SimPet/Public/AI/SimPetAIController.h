// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SimPetAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn *InPawn) override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree *BehaviorTreeAsset;
};
