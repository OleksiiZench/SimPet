// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"

#include "SimPetEnumTypes.h"

#include "SimPetAnimalSubsystem.generated.h"

class ASimPetAnimal;
class ASimPetSpawnPoint;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterSpawnPoint(ASimPetSpawnPoint *SpawnPoint);
	
	void SpawnAnimal(ESimPetAnimals AnimalType);
	
	void MoveAnimalToForest();
	void MoveAnimalToOwner();
	
	const TArray<ASimPetSpawnPoint *> &GetAllSpawnPoints() const;
	
	int32 GetNumberOwnerAnimalsCertainType(ESimPetAnimals AnimalType) const;
	int32 GetTotalNumberOwnerAnimals() const;
	int32 GetTotalNumberWildAnimals() const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TArray<ASimPetSpawnPoint *> AllSpawnPoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TMap<ASimPetAnimal *, ASimPetSpawnPoint *> AnimalToSpawnPointMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TArray<ASimPetAnimal *> OwnerAnimals;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Stats", meta = (AllowPrivateAccess = "true"))
	TArray<ASimPetAnimal *> WildAnimals;
	
	ASimPetSpawnPoint *GetSpawnPointInOwner();
	ASimPetSpawnPoint *GetSpawnPointInForest();
	ASimPetSpawnPoint *FindAvailibleSpawnPointByTag(const FGameplayTag &LocationTag);
	
	void BindAnimalToSpawnPoint(ASimPetAnimal *Animal, ASimPetSpawnPoint *AnimalSpawnPoint);
	void UnbindAnimalFromSpawnPoint(ASimPetAnimal *Animal);
	
	TSubclassOf<ASimPetAnimal> GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const;
	
	UFUNCTION()
	void HandleAnimalDied(ASimPetAnimal *DeadAnimal);
};
