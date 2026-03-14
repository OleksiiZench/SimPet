// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetBackPack.h"

#include "Items/SimPetItem.h"

void ASimPetBackPack::SecondaryInteract_Implementation(AActor *InstigatorActor)
{
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
	if (StoredItems.IsEmpty())
	{
		return;
	}
	
	ASimPetItem *ItemToExtract = StoredItems.Pop();
	
	ShowAndDetachItem(ItemToExtract);
	EnablePhysicsAndApplyImpulse(ItemToExtract);
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

void ASimPetBackPack::ShowAndDetachItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Item->SetActorHiddenInGame(false);
	Item->SetActorEnableCollision(ECollisionEnabled::QueryAndPhysics);
	Item->SetActorTickEnabled(true);
}

void ASimPetBackPack::EnablePhysicsAndApplyImpulse(ASimPetItem *Item)
{
	if (UPrimitiveComponent *PrimCompOfItem = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
	{
		PrimCompOfItem->SetSimulatePhysics(true);
		PrimCompOfItem->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		
		FVector ThrowDirection = -GetActorRightVector() + FVector(0.0f, 0.0f, 0.5f);
		ThrowDirection.Normalize();
		
		float ThrowForce = 400.0f;
		
		PrimCompOfItem->AddImpulse(ThrowDirection * ThrowForce, NAME_None, true);
	}
}

