// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetSpawnPoint.h"

#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"

void ASimPetSpawnPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

void ASimPetSpawnPoint::AddGameplayTags(const FGameplayTag& Tag)
{
	if (Tag.IsValid())
		GameplayTags.AddTag(Tag);
}

void ASimPetSpawnPoint::HandleFeedPickedUp(ASimPetAnimalFeed *PickedFeed)
{
	GameplayTags.RemoveTag(SimPetGameplayTags::Spawn_Point_HasFeed);
	
	if (PickedFeed)
		PickedFeed->OnFeedPickedUp.RemoveDynamic(this, &ASimPetSpawnPoint::HandleFeedPickedUp);
}
