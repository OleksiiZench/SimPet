// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "SimPetEnumTypes.h"
#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetAnimalSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="SimPet Animal Settings"))
class SIMPET_API USimPetAnimalSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "SimPet|Animals")
	TMap<ESimPetAnimals, TSubclassOf<ASimPetAnimal>> AnimalClassMap;
};
