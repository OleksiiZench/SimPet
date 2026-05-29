#pragma once

#include "CoreMinimal.h"

#include "Save/Structures/SimPetItemSaveData.h"

#include "SimPetPlayerSaveData.generated.h"

class ASimPetItem;

USTRUCT(BlueprintType)
struct FSimPetPlayerSaveData
{
	GENERATED_BODY()

public:
	FSimPetPlayerSaveData();
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	FSimPetItemSaveData EquippedItemData;
	
	UPROPERTY()
	float Stamina;
	
	UPROPERTY()
	FRotator ControlRotation;
};
