// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetBackPack.h"

#include "Items/SimPetItem.h"

#include "SimPetDebugHelper.h"

void ASimPetBackPack::SecondaryInteract_Implementation(AActor *InstigatorActor)
{
	Debug::Print("SecondaryInteract_Implementation from backpack");
	
	ExtractItem();
}

bool ASimPetBackPack::TryInteractWithAnotherItem(ASimPetItem *TargetItem)
{
	return TryAddItem(TargetItem);
}

bool ASimPetBackPack::TryAddItem(ASimPetItem *Item)
{
	if (Item == nullptr)
		return false;
	
	if (StoredItems.Num() < MaxCapacity)
	{
		HideAndOptimizeItem(Item);
		AttachItemToBackpack(Item);
		
		StoredItems.Add(Item);
		
		return true;
	}
	
	return false;
}

void ASimPetBackPack::ExtractItem()
{
	Debug::Print("ExtractItem from backpack");
}

void ASimPetBackPack::HideAndOptimizeItem(ASimPetItem *Item)
{
	if (UPrimitiveComponent *PrimCompOfItem = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
		PrimCompOfItem->SetSimulatePhysics(false);
	
	Item->SetActorEnableCollision(false);
	Item->SetActorHiddenInGame(true);
	Item->SetActorTickEnabled(false);
}

void ASimPetBackPack::AttachItemToBackpack(ASimPetItem *Item)
{
	USceneComponent *RootCompOfBackpack = GetRootComponent();
	
	if (RootCompOfBackpack)
		Item->AttachToComponent(RootCompOfBackpack, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

