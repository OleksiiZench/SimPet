// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "SimPetEnumTypes.h"

#include "SimPetSaveSubsystem.generated.h"

class USimPetSaveGame;
class USimPetUISubsystem;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void CheckAndUpdateMaxPoints(int32 InCurrentPoints);
	int32 GetMaxPoints() const;
	EGameDifficulty GetDifficulty() const;
	void SaveDifficulty(EGameDifficulty CurrentDifficulty);
	
	void SaveGame();
	
private:
	UPROPERTY()
	USimPetSaveGame *CachedSaveGame;
	
	const FString SaveSlotName = TEXT("SimPet_SaveSlot");
	int32 MaxPoints;
	EGameDifficulty Difficulty;
	
	void CacheSaveGame();
	USimPetUISubsystem *GetUISubsystem();
	
	void LoadMaxPoints();
	void SaveMaxPoints() const;
	
	void LoadDifficulty();
};
