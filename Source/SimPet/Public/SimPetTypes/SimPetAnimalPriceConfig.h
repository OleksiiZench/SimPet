#pragma once

#include "CoreMinimal.h"

#include "SimPetAnimalPriceConfig.generated.h"

USTRUCT(BlueprintType)
struct FSimPetAnimalPriceConfig
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 EasyPrice;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 NormalPrice;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 HardPrice;
};

