// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DataAsset/Economy/SimPetEconomyData.h"
#include "SimPetEnumTypes.h"

#include "SimPetPointsTransactionComponent.generated.h"

class ASimPetPlayerState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetPointsTransactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	int32 GetAnimalPrice(ESimPetAnimals AnimalType);
	
	bool AttemptTransaction(int32 Cost);
	void GeneratePassivePoints();
	void GeneratePenaltyPoints();
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | Economy")
	USimPetEconomyData *EconomyData;
	
private:
	int32 PointsPerHappyTick;
	int32 PointsPerPenalty;
	ASimPetPlayerState *PlayerState;
	
	void CachePlayerState();
};
