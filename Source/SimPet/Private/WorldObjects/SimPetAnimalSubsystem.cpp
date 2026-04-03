// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetAnimalSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"

USimPetAnimalSubsystem::USimPetAnimalSubsystem()
{
}

void USimPetAnimalSubsystem::SpawnAnimal(TSubclassOf<ASimPetAnimal> SpawnedAnimalClass)
{
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
		
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
