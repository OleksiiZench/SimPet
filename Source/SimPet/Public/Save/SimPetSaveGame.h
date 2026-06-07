// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"

#include "Save/Structures/SimPetPlayerSaveData.h"
#include "Save/Structures/SimPetDroppedItemSaveData.h"
#include "Save/Structures/SimPetAnimalSaveData.h"
#include "Save/Structures/SimPetPointsSaveData.h"

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	FSimPetPlayerSaveData PlayerSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TArray<FSimPetDroppedItemSaveData> DroppedItemsSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TMap<FName, FGameplayTagContainer> SpawnPointsTagsSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TArray<FSimPetAnimalSaveData> AnimalsSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	int32 PlayTimeSaveData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	FSimPetPointsSaveData PointsSaveData;
};
