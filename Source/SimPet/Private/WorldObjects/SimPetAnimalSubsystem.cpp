// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetAnimalSubsystem.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"

void USimPetAnimalSubsystem::RegisterSpawnPoint(ASimPetSpawnPoint *SpawnPoint)
{
	if (SpawnPoint && !AllSpawnPoints.Contains(SpawnPoint))
	{
		AllSpawnPoints.Add(SpawnPoint);
	}
}

void USimPetAnimalSubsystem::SpawnAnimal(ESimPetAnimals AnimalType)
{
	TSubclassOf<ASimPetAnimal> SpawnedAnimalClass = GetAnimalClassByAnimalType(AnimalType);
	if (!SpawnedAnimalClass)
	{
		Debug::Print(TEXT("OwnerAnimals doesn't have a corresponding AnimalType!"));
		return;
	}


	ASimPetSpawnPoint *AnimalSpawnPoint = GetSpawnPointForSpawnAnimal();
	if (AnimalSpawnPoint == nullptr)
		return;

	FTransform AnimalTransform = AnimalSpawnPoint->GetTransform();
	ASimPetAnimal *NewAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(*SpawnedAnimalClass, AnimalTransform);

	if (NewAnimal)
	{
		Debug::Print(TEXT("Spawned: ") + (*SpawnedAnimalClass)->GetName());

		OwnerAnimals.Add(NewAnimal);

		BindAnimalToSpawnPoint(NewAnimal, AnimalSpawnPoint);
	}
	
}

void USimPetAnimalSubsystem::MoveAnimalToForest()
{
	ASimPetAnimal *Animal = OwnerAnimals.Pop();
	if (!Animal)
		return;

	WildAnimals.Add(Animal);
	
	UnbindAnimalFromSpawnPoint(Animal);

	Animal->SetActorLocation(FVector(125.0f, 2470.0f, 100.0f));
	Animal->ApplyForestState();
}

void USimPetAnimalSubsystem::MoveAnimalToOwner()
{
	ASimPetAnimal *Animal = WildAnimals.Pop();
	if (!Animal)
		return;

	OwnerAnimals.Add(Animal);
	
	ASimPetSpawnPoint *AnimalSpawnPoint = GetSpawnPointForSpawnAnimal();
	if (AnimalSpawnPoint == nullptr)
		return;
	
	BindAnimalToSpawnPoint(Animal, AnimalSpawnPoint);
	
	FVector LocationSpawnPoint = AnimalSpawnPoint->GetActorLocation();
	
	Animal->SetActorLocation(LocationSpawnPoint);
	Animal->ApplyOwnerState();
}

int32 USimPetAnimalSubsystem::GetNumberAnimalsCertainType(ESimPetAnimals AnimalType) const
{
	if (OwnerAnimals.IsEmpty())
		return 0;
	
	TSubclassOf<ASimPetAnimal> AnimalClassToFind = GetAnimalClassByAnimalType(AnimalType);
	if (!AnimalClassToFind)
	{
		Debug::Print(TEXT("OwnerAnimals doesn't have a corresponding AnimalType!"));
		return -1;
	}
	
	int counter = 0;
	
	for (ASimPetAnimal *OwnerAnimal : OwnerAnimals)
	{
		if (!IsValid(OwnerAnimal))
			continue;
		
		if (OwnerAnimal->IsA(AnimalClassToFind))
		{
			counter++;
		}
	}
	
	return counter;
}

int32 USimPetAnimalSubsystem::GetNumberWildAnimals() const
{
	return WildAnimals.Num();
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::GetSpawnPointForSpawnAnimal()
{
	for (ASimPetSpawnPoint *SpawnPoint : AllSpawnPoints)
	{
		if (!SpawnPoint)
		{
			continue;
		}
			
		FGameplayTagContainer TagContainer;
		SpawnPoint->GetOwnedGameplayTags(TagContainer);
		
		if (!TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForAnimal) ||
			TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasAnimal))
		{
			continue;
		}
		
		return SpawnPoint;
	}
	
	Debug::Print(TEXT("The desired spawn point for the animal was not found!"));
	return nullptr;
}

void USimPetAnimalSubsystem::BindAnimalToSpawnPoint(ASimPetAnimal *Animal, ASimPetSpawnPoint *AnimalSpawnPoint)
{
	if (!Animal || !AnimalSpawnPoint)
		return;

	AnimalSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasAnimal);
			
	AnimalToSpawnPointMap.Add(Animal, AnimalSpawnPoint);
}

void USimPetAnimalSubsystem::UnbindAnimalFromSpawnPoint(ASimPetAnimal *Animal)
{
	if (!Animal)
		return;

	ASimPetSpawnPoint *AnimalSpawnPoint = AnimalToSpawnPointMap.FindRef(Animal);
	if (!AnimalSpawnPoint)
		return;

	AnimalSpawnPoint->RemoveGameplayTags(SimPetGameplayTags::Spawn_Point_HasAnimal);

	AnimalToSpawnPointMap.Remove(Animal);
}

TSubclassOf<ASimPetAnimal> USimPetAnimalSubsystem::GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const
{
	if (const TSubclassOf<ASimPetAnimal> *FoundClassPtr = AnimalClassMap.Find(AnimalType))
	{
		return *FoundClassPtr;
	}
	
	return nullptr;
}
