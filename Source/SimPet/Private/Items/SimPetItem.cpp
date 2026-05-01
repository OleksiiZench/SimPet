// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetItem.h"

#include "Components/SimPetInteractionComponent.h"

ASimPetItem::ASimPetItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CacheStaticMeshComp = nullptr;
}

void ASimPetItem::BeginPlay()
{
	Super::BeginPlay();
	
	CacheStaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
}

void ASimPetItem::Interact_Implementation(AActor *InstigatorActor)
{
	if (USimPetInteractionComponent *InteractionComp = InstigatorActor->FindComponentByClass<USimPetInteractionComponent>())
	{
		InteractionComp->TakeOrDropOrSwapItem(this);
	}
}

// Базова реалізація навмисно залишена порожньою
// Перевизначте її в підкласах, щоб забезпечити додаткові можливості взаємодії
void ASimPetItem::SecondaryInteract_Implementation(AActor *InstigatorActor)
{
}

void ASimPetItem::GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const
{
	TagContainer.AppendTags(GameplayTags);
}

bool ASimPetItem::TryInteractWithAnotherActor(AActor *TargetActor)
{
	return false;
}

void ASimPetItem::NotifyPickedUp()
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetItem::EnablePhysics()
{
	if (CacheStaticMeshComp)
	{
		CacheStaticMeshComp->SetSimulatePhysics(true);
		CacheStaticMeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		CacheStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetItem::DisablePhysics()
{
	if (CacheStaticMeshComp)
	{
		CacheStaticMeshComp->SetSimulatePhysics(false);
		CacheStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
