// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetInteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Interfaces/SimPetInteractable.h"
#include "Items/SimPetItem.h"
#include "Save/Structures/SimPetItemSaveData.h"
#include "Interfaces/SimPetItemContainer.h"

USimPetInteractionComponent::USimPetInteractionComponent()
{
	CachedTakenItem = nullptr;
	HoldPoint = nullptr;
	
	InteractionTraceChanel = TraceTypeQuery2;
}

// ReSharper disable once CppMemberFunctionMayBeConst
AActor *USimPetInteractionComponent::DoInteractionTrace()
{
	float InteractionDistance = 250.0f;
	float InteractionRadius = 1.0f;
	
	APawn *Player = Cast<APawn>(GetOwner());
	if (!Player)
		return nullptr;
	
	FVector StartLocation = Player->GetPawnViewLocation();
	FVector EndLocation = StartLocation + Player->GetViewRotation().Vector() * InteractionDistance;

	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		InteractionRadius,
		InteractionTraceChanel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (bHit)
	{
		AActor *HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->Implements<USimPetInteractable>())
			return HitActor;
	}
	
	return nullptr;
}

bool USimPetInteractionComponent::TryUseEquippedItemOn(AActor *TargetActor)
{
	if (!IsValid(CachedTakenItem) || CachedTakenItem == TargetActor)
	{
		return false;
	}
	
	bool bWasUsed = CachedTakenItem->TryInteractWithAnotherActor(TargetActor);
	
	if (!IsValid(CachedTakenItem) || CachedTakenItem->GetAttachParentActor() != GetOwner())
	{
		CachedTakenItem = nullptr;
	}
	
	return bWasUsed;
}

void USimPetInteractionComponent::TakeOrDropOrSwapItem(AActor *Actor)
{
	ASimPetItem *Item = Cast<ASimPetItem>(Actor);
	
	if (!Item)
		return;
	
	if (IsValid(CachedTakenItem) && CachedTakenItem == Item)
	{
		DropItem(Item);
	}
	else if (!IsValid(CachedTakenItem))
	{
		TakeItem(Item);
	}
	else
	{
		DropItem(CachedTakenItem);
		TakeItem(Item);
	}
}

void USimPetInteractionComponent::SetHoldPoint(USceneComponent *InHoldPoint)
{
	if (InHoldPoint)
	{
		HoldPoint = InHoldPoint;
	}
}

FSimPetItemSaveData USimPetInteractionComponent::GetSaveDataEquippedItem() const
{
	FSimPetItemSaveData ItemSaveData;
	
	if (IsValid(CachedTakenItem))
	{
		ItemSaveData.ItemClass = CachedTakenItem->GetClass();
	
		if (CachedTakenItem->Implements<USimPetItemContainer>())
		{
			ISimPetItemContainer::Execute_GetContainerPayloadClasses(CachedTakenItem, ItemSaveData.PayloadClasses);
		}
	}
	
	return ItemSaveData;
}

void USimPetInteractionComponent::TakeItem(ASimPetItem *Item)
{
	Item->DisablePhysics();
	
	if (HoldPoint)
		Item->AttachToComponent(HoldPoint, GetRuleForAttachingItems());
	
	CachedTakenItem = Item;
}

void USimPetInteractionComponent::DropItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->EnablePhysics();
	
	CachedTakenItem = nullptr;
}

FAttachmentTransformRules USimPetInteractionComponent::GetRuleForAttachingItems()
{
	return FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
}