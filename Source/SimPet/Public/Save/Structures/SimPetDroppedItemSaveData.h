#pragma once

#include "CoreMinimal.h"

#include "Save/Structures/SimPetItemSaveData.h"

#include "SimPetDroppedItemSaveData.generated.h"

USTRUCT(BlueprintType)
struct FSimPetDroppedItemSaveData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	FTransform Transform;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	FSimPetItemSaveData ItemData;
};
