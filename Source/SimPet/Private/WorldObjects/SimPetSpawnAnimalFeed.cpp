// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetSpawnAnimalFeed.h"

#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"

#include "SimPetDebugHelper.h"

ASimPetSpawnAnimalFeed::ASimPetSpawnAnimalFeed()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASimPetSpawnAnimalFeed::OnZoneEntered);
}

void ASimPetSpawnAnimalFeed::OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Debug::Print(TEXT("ASimPetSpawnAnimalFeed::OnZoneEntered()"));
	
	if (SpawnFeedClass)
	{
		TArray<AActor *> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
		
		// 1. Проходимо по всіх ASimPetSpawnPoint
		for (AActor *Actor : FoundActors)
		{
			ASimPetSpawnPoint *FeedSpawnPoint = Cast<ASimPetSpawnPoint>(Actor);
			
			if (!FeedSpawnPoint)
				continue;
			
			// 2. Отримуємо теги точки
			FGameplayTagContainer TagContainer;
		
			if (FeedSpawnPoint)
				FeedSpawnPoint->GetOwnedGameplayTags(TagContainer);
		
			// 3. Точка має бути SpawnPoint.Spawn_Point_ForFeed та не мати іншого корму
			if (TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForFeed) && !TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasFeed))
			{
				FTransform FeedTransform = FeedSpawnPoint->GetTransform();

				ASimPetAnimalFeed * NewFeed = GetWorld()->SpawnActor<ASimPetAnimalFeed>(*SpawnFeedClass, FeedTransform);

				if (NewFeed)
				{
					Debug::Print(TEXT("Spawned: ") + (*SpawnFeedClass)->GetName());
			
					FeedSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasFeed);
				}
			}
		}
	}
}
