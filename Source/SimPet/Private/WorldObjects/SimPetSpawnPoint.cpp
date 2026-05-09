// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetSpawnPoint.h"

#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"
#include "Subsystems/SimPetAnimalSubsystem.h"

void ASimPetSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterSpawnPointInAnimalSubsystem();
}

void ASimPetSpawnPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

void ASimPetSpawnPoint::AddGameplayTags(const FGameplayTag& Tag)
{
	if (Tag.IsValid())
		GameplayTags.AddTag(Tag);
}

void ASimPetSpawnPoint::RemoveGameplayTags(const FGameplayTag &Tag)
{
	if (Tag.IsValid())
		GameplayTags.RemoveTag(Tag);
}

void ASimPetSpawnPoint::HandleFeedPickedUp(ASimPetAnimalFeed *PickedFeed)
{
	ClearFeedTag();
	
	if (PickedFeed)
		PickedFeed->OnFeedPickedUp.RemoveDynamic(this, &ASimPetSpawnPoint::HandleFeedPickedUp);
}

void ASimPetSpawnPoint::HandleFeedDestroyed(AActor *DestroyedActor)
{
	ClearFeedTag();
	
	if (DestroyedActor)
	{
		DestroyedActor->OnDestroyed.RemoveDynamic(this, &ASimPetSpawnPoint::HandleFeedDestroyed);
	}
}

void ASimPetSpawnPoint::RegisterSpawnPointInAnimalSubsystem()
{
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem)
	{
		AnimalSubsystem->RegisterSpawnPoint(this);
	}
}

void ASimPetSpawnPoint::ClearFeedTag()
{
	GameplayTags.RemoveTag(SimPetGameplayTags::Spawn_Point_HasFeed);
}
