// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetItem.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

#include "Interfaces/SimPetItemContainer.h"
#include "Subsystems/SimPetItemSubsystem.h"

ASimPetItem::ASimPetItem()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CurrentItemState = ESimPetItemState::Dropped;
	CacheStaticMeshComp = nullptr;
}

void ASimPetItem::BeginPlay()
{
	Super::BeginPlay();
	
	CacheStaticMeshComp = FindComponentByClass<UStaticMeshComponent>();
	
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem)
	{
		ItemSubsystem->RegisterActiveItem(this);
	}
}

void ASimPetItem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem)
	{
		ItemSubsystem->UnregisterActiveItem(this);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Базова реалізація навмисно залишена порожньою
// Перевизначте її в підкласах, щоб забезпечити додаткові можливості взаємодії
void ASimPetItem::Interact_Implementation(AActor *InstigatorActor)
{
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

ESimPetItemState ASimPetItem::GetItemState() const
{
	return CurrentItemState;
}

void ASimPetItem::SetItemState(const ESimPetItemState NewItemState)
{
	CurrentItemState = NewItemState;
}

FSimPetItemSaveData ASimPetItem::GetSaveData() const
{
	FSimPetItemSaveData SaveData;
	
	SaveData.ItemClass = GetClass();
	
	if (Implements<USimPetItemContainer>())
	{
		ISimPetItemContainer::Execute_GetContainerPayloadClasses(this, SaveData.PayloadClasses);
	}
	
	return SaveData;
}
