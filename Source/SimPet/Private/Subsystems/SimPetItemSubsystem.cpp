// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetItemSubsystem.h"

#include "Core/Settings/SimPetItemSettings.h"
#include "Items/SimPetAnimalFeed.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"

void USimPetItemSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	CacheSpawnFeedClassFromItemSettings();
}

void USimPetItemSubsystem::SpawnFeedOnRelevantPoints()
{
	if (CachedSpawnFeedClass == nullptr)
		return;
	
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem == nullptr)
		return;
	
	const TArray<ASimPetSpawnPoint *> &AllSpawnPoints = AnimalSubsystem->GetAllSpawnPoints();

	for (ASimPetSpawnPoint *SpawnPoint : AllSpawnPoints)
	{
		if (!SpawnPoint)
			continue;
			
		FGameplayTagContainer TagContainer;
		SpawnPoint->GetOwnedGameplayTags(TagContainer);
			
		if (!TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForFeed) || 
			 TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasFeed))
		{
			continue;
		}
		
		FTransform FeedTransform = SpawnPoint->GetTransform();
		ASimPetAnimalFeed *NewFeed = GetWorld()->SpawnActor<ASimPetAnimalFeed>(CachedSpawnFeedClass, FeedTransform);
		if (NewFeed)
		{
			SpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasFeed);
			
			NewFeed->OnFeedPickedUp.AddDynamic(SpawnPoint, &ASimPetSpawnPoint::HandleFeedPickedUp);
			NewFeed->OnDestroyed.AddDynamic(SpawnPoint, &ASimPetSpawnPoint::HandleFeedDestroyed);
		}
	}
}

void USimPetItemSubsystem::CacheSpawnFeedClassFromItemSettings()
{
	const USimPetItemSettings *Settings = GetDefault<USimPetItemSettings>();
	if (Settings)
	{
		CachedSpawnFeedClass = Settings->SpawnFeedClass;
	}
}
