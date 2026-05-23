// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SimPetEnumTypes.h"

#include "SimPetSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	USimPetSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "SimPet|Stats")
	int32 MaxPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|Stats")
	EGameDifficulty DifficultyIndex;
};
