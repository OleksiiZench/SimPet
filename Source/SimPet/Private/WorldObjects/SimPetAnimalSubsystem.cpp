// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetAnimalSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"

void USimPetAnimalSubsystem::SpawnAnimal(ESimPetAnimals AnimalType)
{
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
	
	TSubclassOf<ASimPetAnimal> SpawnedAnimalClass = GetAnimalClassByAnimalType(AnimalType);
	if (!SpawnedAnimalClass)
	{
		Debug::Print(TEXT("OwnerAnimals doesn't have a corresponding AnimalType!"));
		return;
	}
	
	// 1. Беремо першу ліпшу ASimPetSpawnPoint
	for (AActor *Actor : FoundActors)
	{
		ASimPetSpawnPoint *AnimalSpawnPoint = Cast<ASimPetSpawnPoint>(Actor);
		
		if (!AnimalSpawnPoint)
		{
			continue;
		}
			
		// 2. Отримуємо теги точки
		FGameplayTagContainer TagContainer;
		AnimalSpawnPoint->GetOwnedGameplayTags(TagContainer);
		
		// 3. Точка має бути SpawnPoint.ForAnimal та не мати тварину
		if (!TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForAnimal) ||
			TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasAnimal))
		{
			continue;
		}
		
		FTransform AnimalTransform = AnimalSpawnPoint->GetTransform();
		ASimPetAnimal *NewAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(*SpawnedAnimalClass, AnimalTransform);

		if (NewAnimal)
		{
			Debug::Print(TEXT("Spawned: ") + (*SpawnedAnimalClass)->GetName());
			
			OwnerAnimals.Add(NewAnimal);
			
			AnimalSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasAnimal);

			break;
		}
	}
}

void USimPetAnimalSubsystem::MoveAnimalToForest()
{
	ASimPetAnimal *Animal = OwnerAnimals.Pop();
	WildAnimals.Add(Animal);
	
	Animal->SetActorLocation(FVector(125.0f, 2470.0f, 100.0f));
	Animal->ApplyForestState();
}

void USimPetAnimalSubsystem::MoveAnimalToOwner()
{
	ASimPetAnimal *Animal = WildAnimals.Pop();
	OwnerAnimals.Add(Animal);
	
	Animal->SetActorLocation(FVector(0.0f, 0.0f, 0.0f));
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

TSubclassOf<ASimPetAnimal> USimPetAnimalSubsystem::GetAnimalClassByAnimalType(ESimPetAnimals AnimalType) const
{
	if (const TSubclassOf<ASimPetAnimal> *FoundClassPtr = AnimalClassMap.Find(AnimalType))
	{
		return *FoundClassPtr;
	}
	
	return nullptr;
}
