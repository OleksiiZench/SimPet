// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetInteractionComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Characters/SimPetPlayer.h"
#include "Interfaces/SimPetInteractable.h"
#include "Items/SimPetItem.h"

AActor *USimPetInteractionComponent::DoInteractionTrace()
{
	float InteractionDistance = 250.0f;
	float InteractionRadius = 15.0f;
	
	ASimPetPlayer *Player = Cast<ASimPetPlayer>(GetOwner());
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
		ETraceTypeQuery::TraceTypeQuery2,
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

bool USimPetInteractionComponent::TryItemInteraction(ASimPetItem *TargetItem)
{
	if (!bHandsFull || CashedTakenItem == nullptr || CashedTakenItem == TargetItem)
	{
		return false;
	}
	
	return CashedTakenItem->TryInteractWithAnotherItem(TargetItem);
}

void USimPetInteractionComponent::TakeOrDropItem(ASimPetItem *Item)
{
	if (bHandsFull && CashedTakenItem == Item)
	{
		DropItem(Item);
	}
	else if (!bHandsFull)
	{
		TakeItem(Item);
	}
	else
	{
		DropItem(CashedTakenItem);
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

void USimPetInteractionComponent::TakeItem(ASimPetItem *Item)
{
	Item->DisablePhysics();
	Item->AttachToComponent(HoldPoint, GetRuleForAttachingItems());
	
	CashedTakenItem = Item;
	bHandsFull = true;
}

void USimPetInteractionComponent::DropItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->EnablePhysics();
	
	CashedTakenItem = nullptr;
	bHandsFull = false;
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