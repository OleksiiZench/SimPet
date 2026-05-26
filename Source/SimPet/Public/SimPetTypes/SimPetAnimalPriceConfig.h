#pragma once

#include "CoreMinimal.h"

#include "SimPetEnumTypes.h"

#include "SimPetAnimalPriceConfig.generated.h"

USTRUCT(BlueprintType)
struct FSimPetAnimalPriceConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TMap<EGameDifficulty, int32> PriceByDifficulty;
};

