// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Save/Structures/SimPetItemSaveData.h"
#include "Save/Structures/SimPetDroppedItemSaveData.h"

#include "SimPetItemSubsystem.generated.h"

class ASimPetAnimalFeed;
class ASimPetItem;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetItemSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void SpawnFeedOnRelevantPoints();
	ASimPetItem *RestoreItemFromSaveData(const FSimPetItemSaveData &ItemSaveData, const FTransform &SpawnTransform);
	
	TArray<FSimPetDroppedItemSaveData> GetDroppedItemsSaveData() const;
	void RestoreDroppedItemsFromSaveData(const TArray<FSimPetDroppedItemSaveData> &SavedDroppedItems);
private:
	void RestoreItemPayload(ASimPetItem *ContainerItem, const TArray<TSubclassOf<ASimPetItem>> &PayloadClasses, const FTransform &SpawnTransform);
	ASimPetItem *SpawnItem(TSubclassOf<ASimPetItem> ItemClass, FTransform Transform);
	
	void CacheSpawnFeedClassFromItemSettings();
	
	TSubclassOf<ASimPetAnimalFeed> CachedSpawnFeedClass;
};
