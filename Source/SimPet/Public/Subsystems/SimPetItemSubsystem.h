// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "Save/Structures/SimPetItemSaveData.h"
#include "Save/Structures/SimPetDroppedItemSaveData.h"

#include "SimPetItemSubsystem.generated.h"

class ASimPetAnimalFeed;
class ASimPetItem;
class ASimPetSpawnPoint;

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
	void SpawnFeedOnSinglePoint(ASimPetSpawnPoint *SpawnPoint);
	ASimPetItem *RestoreItemFromSaveData(const FSimPetItemSaveData &ItemSaveData, const FTransform &SpawnTransform);
	
	TArray<FSimPetDroppedItemSaveData> GetDroppedItemsSaveData() const;
	void RestoreDroppedItemsFromSaveData(const TArray<FSimPetDroppedItemSaveData> &SavedDroppedItems);
	
	void RegisterActiveItem(ASimPetItem *ItemToRegister);
	void UnregisterActiveItem(ASimPetItem *ItemToRemove);
private:
	UPROPERTY()
	TSubclassOf<ASimPetAnimalFeed> CachedSpawnFeedClass;
	
	UPROPERTY()
	TArray<ASimPetItem *> AllActiveItems;
	
	void RestoreItemPayload(ASimPetItem *ContainerItem, const TArray<TSubclassOf<ASimPetItem>> &PayloadClasses, const FTransform &SpawnTransform);
	ASimPetItem *SpawnItem(TSubclassOf<ASimPetItem> ItemClass, FTransform Transform);
	
	void ClearDroppedItemsFromLevel();
	
	void CacheSpawnFeedClassFromItemSettings();
};
