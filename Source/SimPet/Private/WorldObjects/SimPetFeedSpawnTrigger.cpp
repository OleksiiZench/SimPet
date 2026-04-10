// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFeedSpawnTrigger.h"

#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"
#include "Characters/SimPetPlayer.h"

#include "SimPetDebugHelper.h"

void ASimPetFeedSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnZoneEntered);
	
	FindAllSpawnPoints();
}

void ASimPetFeedSpawnTrigger::OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(ASimPetPlayer::StaticClass()))
	{// OtherActor - має бути гравцем
		return;
	}
	
	if (SpawnFeedClass)
	{
		for (ASimPetSpawnPoint *FeedSpawnPoint : FeedSpawnPoints)
		{
			if (!FeedSpawnPoint)
				continue;
			
			// Отримуємо теги точки
			FGameplayTagContainer TagContainer;
			FeedSpawnPoint->GetOwnedGameplayTags(TagContainer);
			
			if (!TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForFeed) || 
				 TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasFeed))
			{// Точка має бути SpawnPoint.Spawn_Point_ForFeed та не мати іншого корму
				continue;
			}
			
			FTransform FeedTransform = FeedSpawnPoint->GetTransform();
			ASimPetAnimalFeed *NewFeed = GetWorld()->SpawnActor<ASimPetAnimalFeed>(*SpawnFeedClass, FeedTransform);

			if (NewFeed)
			{
				FeedSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasFeed);
				NewFeed->OnFeedPickedUp.AddDynamic(FeedSpawnPoint, &ASimPetSpawnPoint::HandleFeedPickedUp);
			}
		}
	}
}

void ASimPetFeedSpawnTrigger::FindAllSpawnPoints()
{
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
	
	for (AActor *Actor : FoundActors)
	{
		ASimPetSpawnPoint *FeedSpawnPoint = Cast<ASimPetSpawnPoint>(Actor);
			
		if (!FeedSpawnPoint)
			continue;
		
		FeedSpawnPoints.Add(FeedSpawnPoint);
	}
}
