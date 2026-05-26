// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SimPetEnumTypes.h"
#include "SimPetTypes/SimPetAnimalPriceConfig.h"

#include "SimPetEconomyData.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetEconomyData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimPet | Economy")
	TMap<ESimPetAnimals, FSimPetAnimalPriceConfig> AnimalPrices;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimPet | Economy")
	int32 HappyTickReward;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimPet | Economy")
	int32 PenaltyPoints;
};
