#pragma once

#include "CoreMinimal.h"

#include "SimPetAnimalSaveData.generated.h"

class ASimPetAnimal;

USTRUCT(BlueprintType)
struct FSimPetAnimalSaveData
{
	GENERATED_BODY()
	
public:
	FSimPetAnimalSaveData();
	
	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	TSubclassOf<ASimPetAnimal> AnimalClass;
	
	UPROPERTY()
	float TimeSinceLastMeal;
	
	UPROPERTY()
	float TimeSinceLastClean;
	
	UPROPERTY()
	bool bIsOwned;
	
	UPROPERTY()
	FName BoundSpawnPointName;
};
