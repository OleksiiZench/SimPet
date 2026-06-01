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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TSubclassOf<ASimPetItem> ItemClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|Save Data")
	TArray<TSubclassOf<ASimPetItem>> PayloadClasses;
};
