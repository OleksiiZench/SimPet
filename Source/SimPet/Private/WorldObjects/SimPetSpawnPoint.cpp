// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetSpawnPoint.h"

void ASimPetSpawnPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

void ASimPetSpawnPoint::AddGameplayTags(const FGameplayTag& Tag)
{
	if (Tag.IsValid())
		GameplayTags.AddTag(Tag);
}
