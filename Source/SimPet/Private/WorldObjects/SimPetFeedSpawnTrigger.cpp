// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFeedSpawnTrigger.h"

#include "Components/ShapeComponent.h"

#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"
#include "Characters/SimPetPlayer.h"
#include "WorldObjects/SimPetAnimalSubsystem.h"

void ASimPetFeedSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnZoneEntered);
}

void ASimPetFeedSpawnTrigger::OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(ASimPetPlayer::StaticClass()))
		return;
	
	if (SpawnFeedClass == nullptr)
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
		ASimPetAnimalFeed *NewFeed = GetWorld()->SpawnActor<ASimPetAnimalFeed>(*SpawnFeedClass, FeedTransform);

		if (NewFeed)
		{
			SpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasFeed);
			NewFeed->OnFeedPickedUp.AddDynamic(SpawnPoint, &ASimPetSpawnPoint::HandleFeedPickedUp);
		}
	}
}
