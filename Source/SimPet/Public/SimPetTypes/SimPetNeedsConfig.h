#pragma once

#include "CoreMinimal.h"

#include "SimPetNeedsConfig.generated.h"

USTRUCT(BlueprintType)
struct FSimPetNeedsConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
    float SecondsPerGameHour = 1.0f;
	
    UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
    float HungryThresholdHours = 8.0f;

    UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
    float DeathThresholdHours = 24.0f;

    UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
    float DirtyThresholdHours = 12.0f;
};

