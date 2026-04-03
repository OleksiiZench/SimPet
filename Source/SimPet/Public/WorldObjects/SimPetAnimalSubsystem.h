// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SimPetAnimalSubsystem.generated.h"

class ASimPetAnimal;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	USimPetAnimalSubsystem();
	
	void SpawnAnimal(TSubclassOf<ASimPetAnimal> AnimalClass);
	
	int32 GetNumberAnimalsCertainType();
	
private:
	TArray<ASimPetAnimal *> OwnerAnimals;
};
