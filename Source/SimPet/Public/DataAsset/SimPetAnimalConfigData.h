// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SimPetTypes/SimPetNeedsConfig.h"

#include "SimPetAnimalConfigData.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Difficulty Config")
	FSimPetNeedsConfig EasyConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Difficulty Config")
	FSimPetNeedsConfig NormalConfig;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Difficulty Config")
	FSimPetNeedsConfig HardConfig;
};
