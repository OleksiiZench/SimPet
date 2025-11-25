// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"

#include "SimPetBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASimPetBaseGameMode();

	UFUNCTION(BlueprintCallable, Category = "SimPet|Scoring")
	void AddScore(int32 PointsToAdd);

	UFUNCTION(BlueprintPure, Category = "SimPet|Scoring")
	int32 GetScore() const { return TotalScore; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Scoring")
	int32 TotalScore;
};
