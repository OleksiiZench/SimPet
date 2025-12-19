// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Animals/SimPetAnimal.h"
#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"

#include "SimPetDebugHelper.h"

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
	Debug::Print(__func__);

	if (TSubclassOf<ASimPetAnimal> *FoundClassAnimal = AnimalClassMap.Find(CurrentAnimal))
	{
		TArray<AActor *> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
		
		ASimPetSpawnPoint *AnimalSpawnPoint = nullptr;
		
		// 1. Беремо перешу ліпшу ASimPetSpawnPoint
		for (AActor *Actor : FoundActors)
		{
			if (ASimPetSpawnPoint *СastAnimalSpawnPoint = Cast<ASimPetSpawnPoint>(Actor))
			{
				AnimalSpawnPoint = СastAnimalSpawnPoint;
			}
			else
				continue;
			
			// 2. Отримуємо теги точки
			FGameplayTagContainer TagContainer;
		
			if (AnimalSpawnPoint)
				AnimalSpawnPoint->GetOwnedGameplayTags(TagContainer);
		
			// 3. Точка має бути SpawnPoint та не мати тварину
			if (TagContainer.HasTag(SimPetGameplayTags::Spawn_Point) && !TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasAnimal))
			{
				FTransform AnimalTransform = AnimalSpawnPoint->GetTransform();

				ASimPetAnimal * NewAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(*FoundClassAnimal, AnimalTransform);

				if (NewAnimal)
				{
					Debug::Print(TEXT("Spawned: ") + (*FoundClassAnimal)->GetName());
			
					AnimalSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasAnimal);
				
					break;
				}
				else
					continue;
			}
			else
				continue;
		}
	}
}
