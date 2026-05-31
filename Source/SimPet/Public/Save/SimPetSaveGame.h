// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Save/Structures/SimPetPlayerSaveData.h"

#include "SimPetSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "SimPet|Save Data")
	FSimPetPlayerSaveData PlayerSaveData;
};
