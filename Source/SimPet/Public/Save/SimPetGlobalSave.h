// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SimPetTypes/SimPetEnumTypes.h"

#include "SimPetGlobalSave.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetGlobalSave : public USaveGame
{
	GENERATED_BODY()
		
public:
	USimPetGlobalSave();

	UPROPERTY(VisibleAnywhere, Category = "SimPet|Stats")
	int32 MaxPoints;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|Stats")
	EGameDifficulty Difficulty;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|SaveData")
	bool bHasSeenHelp;
};
