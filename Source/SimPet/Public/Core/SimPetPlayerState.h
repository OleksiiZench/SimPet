// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Save/Structures/SimPetPointsSaveData.h"

#include "SimPetPlayerState.generated.h"

class USimPetSaveSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPointsChanged, int, Points);

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASimPetPlayerState();
	
	virtual void BeginPlay() override;
	
	bool HasRequiredPoints(int32 Points) const;
	void SpendPoints(int32 Points);
	void AddPoints(int32 Points);
	void ApplyPenalty(int32 PenaltyAmount);
	
	bool HasBoughtFirstAnimal() const;
	void MarkFirstAnimalBought();
	
	int32 GetCurrentPoints() const;
	
	FSimPetPointsSaveData GetSaveData() const;
	void RestoreFromSaveData(const FSimPetPointsSaveData &SaveData);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPointsChanged OnPointsChanged;
	
private:
	UPROPERTY()
	USimPetSaveSubsystem *CachedSaveSubsystem;
	
	int32 CurrentPoints;
	
	bool bHasBoughtFirstAnimal;
	
	void UpdateCurrentPoints(int32 NewCurrentPoints);
	
	void CacheSaveSubsystem();
};
