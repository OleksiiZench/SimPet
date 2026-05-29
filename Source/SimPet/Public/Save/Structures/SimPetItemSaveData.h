#pragma once

#include "CoreMinimal.h"

#include "SimPetItemSaveData.generated.h"

class ASimPetItem;

USTRUCT(BlueprintType)
struct FSimPetItemSaveData
{
	GENERATED_BODY()

public:
	FSimPetItemSaveData();
	
	UPROPERTY()
	TSubclassOf<ASimPetItem> ItemClass;
	
	UPROPERTY()
	TArray<TSubclassOf<ASimPetItem>> PayloadClasses;
};
