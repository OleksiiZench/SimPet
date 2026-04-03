// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SimPetEnumTypes.h"

#include "SimPetAnimalSubsystem.generated.h"

class ASimPetAnimal;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SIMPET_API USimPetAnimalSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void SpawnAnimal(ESimPetAnimals AnimalType);
	
	int32 GetNumberAnimalsCertainType(ESimPetAnimals AnimalType) const;
	int32 GetNumberWildAnimals() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|AnimalSettings")
	TMap<ESimPetAnimals, TSubclassOf<ASimPetAnimal>> AnimalClassMap;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TArray<ASimPetAnimal *> OwnerAnimals;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TArray<ASimPetAnimal *> WildAnimals;
	
	TSubclassOf<ASimPetAnimal> GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const;
};
