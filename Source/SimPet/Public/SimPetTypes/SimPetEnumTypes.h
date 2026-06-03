#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "SimPetEnumTypes.generated.h"

UENUM(BlueprintType)
enum class ESimPetAnimalState : uint8
{
    Happy     UMETA(DisplayName = "Happy"),
    Tired     UMETA(DisplayName = "Tired"),
    Dead      UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ESimPetAnimals : uint8
{
    EA_Canary	UMETA(DisplayName = "Canary"),
    EA_Dog		UMETA(DisplayName = "Dog"),
    EA_Lizard	UMETA(DisplayName = "Lizard")
};

UENUM(BlueprintType)
enum class EGameDifficulty : uint8
{
    Easy     UMETA(DisplayName = "Easy"),
    Normal   UMETA(DisplayName = "Normal"),
    Hard     UMETA(DisplayName = "Hard")
};

UENUM(BlueprintType)
enum class ESimPetItemState : uint8
{
    Dropped                UMETA(DisplayName = "Dropped on Level"),
    Equipped               UMETA(DisplayName = "Equipped by Actor"),
    InContainer            UMETA(DisplayName = "Inside a Container"),
    ManagedBySpawnPoint    UMETA(DisplayName = "Managed by SpawnPoint")
};

UCLASS()
class SIMPET_API USimPetEnumTypes : public UObject
{
    GENERATED_BODY()
};
