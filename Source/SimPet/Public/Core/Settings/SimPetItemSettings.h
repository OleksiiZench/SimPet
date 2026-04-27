// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "SimPetItemSettings.generated.h"

class ASimPetAnimalFeed;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="SimPet Item Settings"))
class SIMPET_API USimPetItemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "SimPet|Items")
	TSubclassOf<ASimPetAnimalFeed> SpawnFeedClass;
};
