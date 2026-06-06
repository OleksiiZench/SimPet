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
		SubscribeToAnimalDeath(NewAnimal);
		AddOwnerAnimalAndNotify(NewAnimal);
		BindAnimalToSpawnPoint(NewAnimal, AnimalSpawnPoint);
	}
	
	return NewAnimal;
}

void USimPetAnimalSubsystem::HandleWasteCleaned()
{
	const TArray<ASimPetAnimal *> &Animals = GetOwnerAnimal();
	
	for (ASimPetAnimal *Animal : Animals)
	{
		if (IsValid(Animal) && Animal->NeedsCleaning())
		{
			Animal->CleanAnimal();
			
			break;
		}
	}
}

void USimPetAnimalSubsystem::MoveAnimalToForest()
{
	if (OwnerAnimals.IsEmpty())
		return;
	
	ASimPetAnimal *Animal = OwnerAnimals.Pop();
	if (!Animal)
		return;

	AddWildAnimalAndNotify(Animal);
	
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

	AddOwnerAnimalAndNotify(Animal);
	
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

TArray<FSimPetAnimalSaveData> USimPetAnimalSubsystem::GetAnimalsSaveData() const
{
	TArray<FSimPetAnimalSaveData> ResultArray;
	
	auto CollectSaveData = [&](const TArray<ASimPetAnimal *> &Animals, bool bIsOwned)
	{
		for (ASimPetAnimal *Animal : Animals)
		{
			if (!IsValid(Animal) || Animal->GetAnimalState() == ESimPetAnimalState::Dead)
				continue;

			FSimPetAnimalSaveData Data = Animal->GetAnimalSaveData();
			Data.bIsOwned = bIsOwned;

			ASimPetSpawnPoint *SpawnPoint = AnimalToSpawnPointMap.FindRef(Animal);
			if (SpawnPoint)
				Data.BoundSpawnPointName = SpawnPoint->GetFName();

			ResultArray.Add(Data);
		}
	};
	
	CollectSaveData(OwnerAnimals, true);
	CollectSaveData(WildAnimals, false);
	
	return ResultArray;
}

void USimPetAnimalSubsystem::RestoreAnimalsFromSave(const TArray<FSimPetAnimalSaveData> &SavedData)
{
	ClearAnimalsFromLevel();
	
	for (const FSimPetAnimalSaveData &Data : SavedData)
	{
		if (!IsValid(Data.AnimalClass))
			continue;

		ASimPetAnimal *SpawnedAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(Data.AnimalClass, Data.Transform);
		if (SpawnedAnimal == nullptr)
			return;

		SpawnedAnimal->RestoreFromSaveData(Data);
		SubscribeToAnimalDeath(SpawnedAnimal);

		if (Data.bIsOwned)
		{
			OwnerAnimals.Add(SpawnedAnimal);
			SpawnedAnimal->ApplyOwnerState();
		}
		else
		{
			WildAnimals.Add(SpawnedAnimal);
			SpawnedAnimal->ApplyForestState();
		}

		ASimPetSpawnPoint *SpawnPoint = FindSpawnPointByName(Data.BoundSpawnPointName);
		if (SpawnPoint)
			BindAnimalToSpawnPoint(SpawnedAnimal, SpawnPoint);
	}
	
	OnAnimalCountChanged.Broadcast();
}

void USimPetAnimalSubsystem::SubscribeToAnimalDeath(ASimPetAnimal *Animal)
{
	if (Animal == nullptr)
		return;
	
	Animal->OnAnimalDied.AddDynamic(this, &ThisClass::HandleAnimalDied);
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::FindSpawnPointByName(FName SpawnPointName) const
{
	if (SpawnPointName == NAME_None)
		return nullptr;

	for (ASimPetSpawnPoint *SpawnPoint : AllSpawnPoints)
	{
		if (SpawnPoint && SpawnPoint->GetFName() == SpawnPointName)
			return SpawnPoint;
	}
	
	return nullptr;
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::GetSpawnPointInOwner()
{
	return FindAvailableSpawnPointByTag(SimPetGameplayTags::Spawn_Point_ForAnimal_InOwner);
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::GetSpawnPointInForest()
{
	return FindAvailableSpawnPointByTag(SimPetGameplayTags::Spawn_Point_ForAnimal_InForest);
}

ASimPetSpawnPoint * USimPetAnimalSubsystem::FindAvailableSpawnPointByTag(const FGameplayTag &LocationTag)
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

TArray<ASimPetAnimal *> USimPetAnimalSubsystem::GetOwnerAnimal() const
{
	return OwnerAnimals;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetAnimalSubsystem::ClearAnimalsFromLevel()
{
	TArray<ASimPetAnimal *> AllAnimals;
	AllAnimals.Append(OwnerAnimals);
	AllAnimals.Append(WildAnimals);
	
	for (int32 i = AllAnimals.Num() - 1; i > 0; --i)
	{
		if (IsValid(AllAnimals[i]))
			AllAnimals[i]->Destroy();
	}
}

void USimPetAnimalSubsystem::AddOwnerAnimalAndNotify(ASimPetAnimal *Animal)
{
	if (Animal == nullptr)
		return;
	
	OwnerAnimals.Add(Animal);
	
	OnAnimalCountChanged.Broadcast();
}

void USimPetAnimalSubsystem::AddWildAnimalAndNotify(ASimPetAnimal *Animal)
{
	if (Animal == nullptr)
		return;
	
	WildAnimals.Add(Animal);
	
	OnAnimalCountChanged.Broadcast();
}

void USimPetAnimalSubsystem::RemoveOwnerAnimalAndNotify(ASimPetAnimal *Animal)
{
	if (Animal == nullptr)
		return;
	
	OwnerAnimals.RemoveSingle(Animal);
	
	OnAnimalCountChanged.Broadcast();
}

void USimPetAnimalSubsystem::RemoveWildAnimalAndNotify(ASimPetAnimal *Animal)
{
	if (Animal == nullptr)
		return;
	
	WildAnimals.RemoveSingle(Animal);
	
	OnAnimalCountChanged.Broadcast();
}

void USimPetAnimalSubsystem::HandleAnimalDied(ASimPetAnimal *DeadAnimal)
{
	if (DeadAnimal == nullptr)
		return;
	
	DeadAnimal->OnAnimalDied.RemoveDynamic(this, &ThisClass::HandleAnimalDied);
	
	UnbindAnimalFromSpawnPoint(DeadAnimal);
	
	if (OwnerAnimals.Contains(DeadAnimal))
		RemoveOwnerAnimalAndNotify(DeadAnimal);
	else if (WildAnimals.Contains(DeadAnimal))
		RemoveWildAnimalAndNotify(DeadAnimal);
}
