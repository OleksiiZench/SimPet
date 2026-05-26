// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DataAsset/Economy/SimPetEconomyData.h"

#include "SimPetPointsTransactionComponent.generated.h"

class ASimPetPlayerState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetPointsTransactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void RegisterAnimalPurchase();
	
	void ConsumePoints(int32 Cost);
	void GeneratePassivePoints();
	void GeneratePenaltyPoints();
	
	int32 GetAnimalPrice(ESimPetAnimals AnimalType) const;	
	bool CanAfford(int32 Cost) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | Economy")
	USimPetEconomyData *EconomyData;
	
private:
	UPROPERTY()
	ASimPetPlayerState *PlayerState;
	
	int32 PointsPerHappyTick;
	int32 PointsPerPenalty;
	
	void InitializeEconomySettings();
	void CachePlayerState();
	
	EGameDifficulty GetDifficulty() const;
};
