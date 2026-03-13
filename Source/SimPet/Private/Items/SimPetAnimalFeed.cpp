// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalFeed.h"

void ASimPetAnimalFeed::Interact_Implementation(AActor *InstigatorActor)
{
	Super::Interact_Implementation(InstigatorActor);
	
	OnFeedPickedUp.Broadcast(this);
}
