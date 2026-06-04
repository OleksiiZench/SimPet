// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Save/Structures/SimPetPlayerSaveData.h"
#include "Save/Structures/SimPetDroppedItemSaveData.h"
#include "SimPetGameplayTags.h"

#include "SimPetSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	FSimPetPlayerSaveData PlayerSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TArray<FSimPetDroppedItemSaveData> DroppedItemsSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TMap<FName, FGameplayTagContainer> SavedSpawnPointsTags;
};
