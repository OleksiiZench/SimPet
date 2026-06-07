#pragma once

#include "CoreMinimal.h"

#include "SimPetPointsSaveData.generated.h"

USTRUCT(BlueprintType)
struct FSimPetPointsSaveData
{
	GENERATED_BODY()

public:
	FSimPetPointsSaveData();
	
	UPROPERTY()
	int32 CurrentPoints;
	
	UPROPERTY()
	bool HasBoughtFirstAnimal;
};
