// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "SimPetPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointsChanged, int, Points);

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPointsChanged OnPointsChanged;
	
private:
	int32 CurrentPoints;
	
	void AddPoints(int32 Points);
	void SpendPoints(int32 Points);
	void ApplyPenalty(int32 PenaltyAmount);
};
