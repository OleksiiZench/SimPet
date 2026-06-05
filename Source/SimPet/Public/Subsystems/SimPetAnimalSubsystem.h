// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"

#include "SimPetTypes/SimPetEnumTypes.h"
#include "Save/Structures/SimPetAnimalSaveData.h"

#include "SimPetAnimalSubsystem.generated.h"

class ASimPetAnimal;
class ASimPetSpawnPoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimalCountChangedSignature);

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterSpawnPoint(ASimPetSpawnPoint *SpawnPoint);
	
	ASimPetAnimal * SpawnAnimal(ESimPetAnimals AnimalType);
	
	void HandleWasteCleaned();
	
	void MoveAnimalToForest();
	void MoveAnimalToOwner();
	
	const TArray<ASimPetSpawnPoint *> &GetAllSpawnPoints() const;
	
	int32 GetNumberOwnerAnimalsCertainType(ESimPetAnimals AnimalType) const;
	int32 GetTotalNumberOwnerAnimals() const;
	int32 GetTotalNumberWildAnimals() const;
	
	TArray<FSimPetAnimalSaveData> GetAnimalsSaveData() const;
	void RestoreAnimalsFromSave(const TArray<FSimPetAnimalSaveData> &SavedData);
	
	FOnAnimalCountChangedSignature OnAnimalCountChanged;
	
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
	ASimPetSpawnPoint *FindAvailableSpawnPointByTag(const FGameplayTag &LocationTag);
	
	void BindAnimalToSpawnPoint(ASimPetAnimal *Animal, ASimPetSpawnPoint *AnimalSpawnPoint);
	void UnbindAnimalFromSpawnPoint(ASimPetAnimal *Animal);
	
	TSubclassOf<ASimPetAnimal> GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const;
	TArray <ASimPetAnimal *> GetOwnerAnimal() const;
	
	void ClearAnimalsFromLevel();
	
	void AddOwnerAnimalAndNotify(ASimPetAnimal *Animal);
	void AddWildAnimalAndNotify(ASimPetAnimal *Animal);
	void RemoveOwnerAnimalAndNotify(ASimPetAnimal *Animal);
	void RemoveWildAnimalAndNotify(ASimPetAnimal *Animal);
	
	UFUNCTION()
	void HandleAnimalDied(ASimPetAnimal *DeadAnimal);
};
