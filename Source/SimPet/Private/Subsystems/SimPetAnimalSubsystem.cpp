// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetAnimalSubsystem.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Characters/Animals/SimPetAnimal.h"
#include "Core/Settings/SimPetAnimalSettings.h"

#include "SimPetDebugHelper.h"


void USimPetAnimalSubsystem::RegisterSpawnPoint(ASimPetSpawnPoint *SpawnPoint)
{
	if (SpawnPoint && !AllSpawnPoints.Contains(SpawnPoint))
	{
		AllSpawnPoints.Add(SpawnPoint);
	}
}

ASimPetAnimal * USimPetAnimalSubsystem::SpawnAnimal(ESimPetAnimals AnimalType)
{
	TSubclassOf<ASimPetAnimal> SpawnedAnimalClass = GetAnimalClassByAnimalType(AnimalType);
	if (!SpawnedAnimalClass)
	{
		Debug::Print(TEXT("OwnerAnimals doesn't have a corresponding AnimalType!"));
		return nullptr;
	}
	
	ASimPetSpawnPoint *AnimalSpawnPoint = GetSpawnPointInOwner();
	if (AnimalSpawnPoint == nullptr)
		return nullptr;

	FTransform AnimalTransform = AnimalSpawnPoint->GetTransform();
	ASimPetAnimal *NewAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(*SpawnedAnimalClass, AnimalTransform);
	if (NewAnimal)
	{
		Debug::Print(TEXT("Spawned: ") + (*SpawnedAnimalClass)->GetName());

		OwnerAnimals.Add(NewAnimal);
		BindAnimalToSpawnPoint(NewAnimal, AnimalSpawnPoint);
		
		return NewAnimal;
	}
	
	return nullptr;
}

void USimPetAnimalSubsystem::MoveAnimalToForest()
{
	if (OwnerAnimals.IsEmpty())
		return;
	
	ASimPetAnimal *Animal = OwnerAnimals.Pop();
	if (!Animal)
		return;

	WildAnimals.Add(Animal);
	
	UnbindAnimalFromSpawnPoint(Animal);
	
	ASimPetSpawnPoint *SpawnPoint = GetSpawnPointInForest();
	if (SpawnPoint == nullptr)
		return;
	
	FVector LocationSpawnPoint = SpawnPoint->GetActorLocation();

	Animal->SetActorLocation(LocationSpawnPoint);
	Animal->ApplyForestState();
}

void USimPetAnimalSubsystem::MoveAnimalToOwner()
{
	if (WildAnimals.IsEmpty())
		return;
	
	ASimPetAnimal *Animal = WildAnimals.Pop();
	if (!Animal)
		return;

	OwnerAnimals.Add(Animal);
	
	ASimPetSpawnPoint *AnimalSpawnPoint = GetSpawnPointInOwner();
	if (AnimalSpawnPoint == nullptr)
		return;
	
	BindAnimalToSpawnPoint(Animal, AnimalSpawnPoint);
	
	FVector LocationSpawnPoint = AnimalSpawnPoint->GetActorLocation();
	
	Animal->SetActorLocation(LocationSpawnPoint);
	Animal->ApplyOwnerState();
}

const TArray<ASimPetSpawnPoint *> & USimPetAnimalSubsystem::GetAllSpawnPoints() const
{
	return AllSpawnPoints;
}

int32 USimPetAnimalSubsystem::GetNumberOwnerAnimalsCertainType(ESimPetAnimals AnimalType) const
{
	if (OwnerAnimals.IsEmpty())
		return 0;
	
	TSubclassOf<ASimPetAnimal> AnimalClassToFind = GetAnimalClassByAnimalType(AnimalType);
	if (!AnimalClassToFind)
	{
		Debug::Print(TEXT("OwnerAnimals doesn't have a corresponding AnimalType!"));
		return -1;
	}
	
	int32 Counter = 0;
	
	for (ASimPetAnimal *OwnerAnimal : OwnerAnimals)
	{
		if (!IsValid(OwnerAnimal))
			continue;
		
		if (OwnerAnimal->IsA(AnimalClassToFind))
		{
			Counter++;
		}
	}
	
	return Counter;
}

int32 USimPetAnimalSubsystem::GetTotalNumberOwnerAnimals() const
{
	return OwnerAnimals.Num();
}

int32 USimPetAnimalSubsystem::GetTotalNumberWildAnimals() const
{
	return WildAnimals.Num();
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::GetSpawnPointInOwner()
{
	return FindAvailibleSpawnPointByTag(SimPetGameplayTags::Spawn_Point_ForAnimal_InOwner);
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::GetSpawnPointInForest()
{
	return FindAvailibleSpawnPointByTag(SimPetGameplayTags::Spawn_Point_ForAnimal_InForest);
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::FindAvailibleSpawnPointByTag(const FGameplayTag &LocationTag)
{
	for (ASimPetSpawnPoint *SpawnPoint : AllSpawnPoints)
	{
		if (!SpawnPoint)
		{
			continue;
		}
			
		FGameplayTagContainer TagContainer;
		SpawnPoint->GetOwnedGameplayTags(TagContainer);
		
		if (!TagContainer.HasTag(LocationTag) ||
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
	
	Animal->OnAnimalDied.AddDynamic(this, &ThisClass::HandleAnimalDied);
	
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

	Animal->OnAnimalDied.RemoveDynamic(this, &ThisClass::HandleAnimalDied);
	
	AnimalToSpawnPointMap.Remove(Animal);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
TSubclassOf<ASimPetAnimal> USimPetAnimalSubsystem::GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const
{
	const USimPetAnimalSettings *Settings = GetDefault<USimPetAnimalSettings>();
	if (Settings)
	{
		if (const TSubclassOf<ASimPetAnimal> *FoundClassPtr = Settings->AnimalClassMap.Find(AnimalType))
		{
			return *FoundClassPtr;
		}
	}
	
	return nullptr;
}

void USimPetAnimalSubsystem::HandleAnimalDied(ASimPetAnimal *DeadAnimal)
{
	if (DeadAnimal == nullptr)
		return;
	
	UnbindAnimalFromSpawnPoint(DeadAnimal);
	
	OwnerAnimals.RemoveSingle(DeadAnimal);
	WildAnimals.RemoveSingle(DeadAnimal);
}
