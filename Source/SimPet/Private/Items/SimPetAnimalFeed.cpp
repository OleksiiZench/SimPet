// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalFeed.h"

#include "Characters/SimPetPlayer.h"

#include "SimPetDebugHelper.h"

void ASimPetAnimalFeed::Interact_Implementation(AActor *InstigatorActor)
{
	Super::Interact_Implementation(InstigatorActor);
	
	AttachToPlayer(InstigatorActor);
	
	OnFeedPickedUp.Broadcast(this);
}

void ASimPetAnimalFeed::AttachToPlayer(AActor *InstigatorActor)
{
	ASimPetPlayer *Player = Cast<ASimPetPlayer>(InstigatorActor);
	
	if (Player)
	{
		Player->TakeOrDropItem(this);
	}
}
