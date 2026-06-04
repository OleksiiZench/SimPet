// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SimPetTypes/SimPetEnumTypes.h"

#include "SimPetSaveSubsystem.generated.h"

class USimPetGlobalSave;
class USimPetUISubsystem;
class USimPetSaveGame;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USimPetSaveSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void CheckAndUpdateMaxPoints(int32 InCurrentPoints);
	int32 GetMaxPoints() const;
	EGameDifficulty GetDifficulty() const;
	void SaveDifficulty(EGameDifficulty CurrentDifficulty);
	
	bool HasAnySaveGame() const;
	void SaveGame();
	void LoadGame();
	
	void RegisterSavableActor(AActor *ActorToRegister);
	void UnregisterSavableActor(AActor *ActorToRemove);
	
	void RequestLoadGame();
	bool IsLoadGameRequested() const;
	void ClearLoadGameRequested();
private:
	UPROPERTY()
	USimPetGlobalSave *CachedGlobalSave;
	
	UPROPERTY()
	USimPetSaveGame *CachedSaveGame;
	
	UPROPERTY()
	TArray<AActor *> RegisteredSavableActors;
	
	bool bLoadGameRequested;
	
	const FString GlobalSaveSlotName = TEXT("SimPet_GlobalSaveSlot");
	const FString SaveGameSlotName = TEXT("SimPet_SaveGameSlot");
	
	int32 MaxPoints;
	EGameDifficulty Difficulty;
	
	void CacheGlobalSave();
	void CacheSaveGame();
	
	USimPetUISubsystem *GetUISubsystem() const;
	
	void LoadMaxPoints();
	void SaveMaxPoints() const;
	
	void LoadDifficulty();
	
	void SaveRegisteredActorsData();
	void SaveDroppedItemsData();
	void SaveAnimalsData();
	bool WriteSaveGameToDisk() const;
	void NotifySaveResult(const bool bIsSuccess);
	
	bool ReadSaveGameFromDisk();
	void LoadRegisteredActorsData();
	void RestoreDroppedItemsData();
	void RestoreAnimalsData();
};
