// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetItem.h"

ASimPetItem::ASimPetItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASimPetItem::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}
