// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetInteractionComponent.h"

#include "Engine/HitResult.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Interfaces/SimPetInteractable.h"
#include "Items/SimPetItem.h"
#include "Save/Structures/SimPetItemSaveData.h"
#include "Subsystems/SimPetItemSubsystem.h"

USimPetInteractionComponent::USimPetInteractionComponent()
{
	CachedTakenItem = nullptr;
	HoldPoint = nullptr;
	
	PickUpTraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	InteractTraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
}

void USimPetInteractionComponent::TryPickUp()
{
	AActor *HitActor = DoInteractionTrace(50.0f, PickUpTraceChannel);
	
	if (IsValid(CachedTakenItem))
	{
		if (HitActor != CachedTakenItem)
			DropItem(CachedTakenItem);
	}
	else if (HitActor)
	{
		TakeItem(Cast<ASimPetItem>(HitActor));
	}
}

void USimPetInteractionComponent::TryInteract()
{
	AActor *HitActor = DoInteractionTrace(1, InteractTraceChannel);
	if (HitActor)
	{ 
		if (!TryUseEquippedItemOn(HitActor))
		{
			ISimPetInteractable::Execute_Interact(HitActor, GetOwner());
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
AActor *USimPetInteractionComponent::DoInteractionTrace(float InteractionRadius, ETraceTypeQuery TraceChannel)
{
	float InteractionDistance = 250.0f;
	
	APawn *Player = Cast<APawn>(GetOwner());
	if (!Player)
		return nullptr;
	
	FVector StartLocation = Player->GetPawnViewLocation();
	FVector EndLocation = StartLocation + Player->GetViewRotation().Vector() * InteractionDistance;

	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	
	if (IsValid(CachedTakenItem))
		ActorsToIgnore.Add(CachedTakenItem);
	
	FHitResult HitResult;

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		InteractionRadius,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
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

void USimPetInteractionComponent::SetHoldPoint(USceneComponent *InHoldPoint)
{
	if (InHoldPoint)
	{
		HoldPoint = InHoldPoint;
	}
}

FSimPetItemSaveData USimPetInteractionComponent::GetSaveDataEquippedItem() const
{
	if (IsValid(CachedTakenItem))
	{
		return CachedTakenItem->GetSaveData();
	}
	
	return FSimPetItemSaveData();
}

void USimPetInteractionComponent::ApplyLoadedDataOfEquippedItem(const FSimPetItemSaveData &ItemSaveData)
{
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem == nullptr)
		return;
	
	ASimPetItem *RestoredItem = ItemSubsystem->RestoreItemFromSaveData(ItemSaveData, GetOwner()->GetActorTransform());
	if (RestoredItem)
	{
		TakeItem(RestoredItem);
	}
}

void USimPetInteractionComponent::TakeItem(ASimPetItem *Item)
{
	Item->DisablePhysics();
	
	if (HoldPoint)
		Item->AttachToComponent(HoldPoint, GetRuleForAttachingItems());
	
	Item->SetItemState(ESimPetItemState::Equipped);
	
	CachedTakenItem = Item;
}

void USimPetInteractionComponent::DropItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->EnablePhysics();
	
	Item->SetItemState(ESimPetItemState::Dropped);
	
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