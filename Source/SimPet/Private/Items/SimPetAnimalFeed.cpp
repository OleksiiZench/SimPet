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
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
	
	ASimPetPlayer *Player = СastInstigatorToPlayer(InstigatorActor); 
	
	if (Player)
		AttachToComponent(Player->ItemHoldPoint, AttachmentRules);
}

ASimPetPlayer* ASimPetAnimalFeed::СastInstigatorToPlayer(AActor *InstigatorActor)
{
	ASimPetPlayer* InstigatorPlayer;
	
	if (InstigatorActor)
	{
		InstigatorPlayer = Cast<ASimPetPlayer>(InstigatorActor);
		return InstigatorPlayer;
	}
	else
		return nullptr;
}
