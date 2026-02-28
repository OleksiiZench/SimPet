// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalFeed.h"
#include "GameFramework/Character.h"

#include "Characters/SimPetPlayer.h"

#include "SimPetDebugHelper.h"

void ASimPetAnimalFeed::Interact_Implementation(AActor *InstigatorActor)
{
	Super::Interact_Implementation(InstigatorActor);
	
	Debug::Print(TEXT("ASimPetAnimalFeed::Interact_Implementation"));
	
	AttachToPlayer(InstigatorActor);
}

void ASimPetAnimalFeed::AttachToPlayer(AActor *InstigatorActor)
{
	ASimPetPlayer *Player = Cast<ASimPetPlayer>(InstigatorActor);
	
	if (Player)
	{
		Player->TakeOrDropItem(this);
	}
}
