// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetItem.h"

ASimPetItem::ASimPetItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASimPetItem::Interact_Implementation(AActor *InstigatorActor)
{
}

void ASimPetItem::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

void ASimPetItem::EnablePhysics()
{
	UStaticMeshComponent *StaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComp)
	{
		StaticMeshComp->SetSimulatePhysics(true);
		StaticMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ASimPetItem::DisablePhysics()
{
	UStaticMeshComponent *StaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComp)
	{
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
