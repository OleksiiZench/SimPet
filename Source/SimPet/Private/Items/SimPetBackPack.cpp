// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetBackPack.h"

#include "Items/SimPetItem.h"

void ASimPetBackPack::SecondaryInteract_Implementation(AActor *InstigatorActor)
{
	ExtractItem();
}

bool ASimPetBackPack::TryAddItemToContainer_Implementation(ASimPetItem *Item)
{
	return TryAddItem(Item);
}

void ASimPetBackPack::GetContainerPayloadClasses_Implementation(TArray<TSubclassOf<ASimPetItem>> &OutPayloadClasses)
{
	OutPayloadClasses.Empty();
	
	for (ASimPetItem *Item : StoredItems)
	{
		if (IsValid(Item))
			OutPayloadClasses.Add(Item->GetClass());
	}
}

bool ASimPetBackPack::TryInteractWithAnotherActor(AActor *TargetActor)
{
	ASimPetItem *Item = Cast<ASimPetItem>(TargetActor);
	
	if (Item == nullptr)
		return false;
	
	return TryAddItem(Item);
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
		
		Item->NotifyPickedUp();
		
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

// ReSharper disable once CppMemberFunctionMayBeStatic
void ASimPetBackPack::HideAndOptimizeItem(ASimPetItem *Item)
{
	if (UPrimitiveComponent *PrimCompOfItem = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
		PrimCompOfItem->SetSimulatePhysics(false);
	
	Item->SetActorEnableCollision(false);
	Item->SetActorHiddenInGame(true);
	Item->SetActorTickEnabled(false);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetBackPack::AttachItemToBackpack(ASimPetItem *Item)
{
	USceneComponent *RootCompOfBackpack = GetRootComponent();
	if (RootCompOfBackpack)
		Item->AttachToComponent(RootCompOfBackpack, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void ASimPetBackPack::ShowAndDetachItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Item->SetActorHiddenInGame(false);
	Item->SetActorEnableCollision(ECollisionEnabled::QueryAndPhysics);
	Item->SetActorTickEnabled(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppParameterMayBeConstPtrOrRef
void ASimPetBackPack::EnablePhysicsAndApplyImpulse(ASimPetItem *Item)
{
	if (Item == nullptr)
		return;
	
	if (UPrimitiveComponent *PrimCompOfItem = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
	{
		PrimCompOfItem->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PrimCompOfItem->SetSimulatePhysics(true);
		PrimCompOfItem->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		
		FVector ThrowDirection = -GetActorRightVector() + FVector(0.0f, 0.0f, 0.5f);
		ThrowDirection.Normalize();
		
		float ThrowForce = 400.0f;
		
		PrimCompOfItem->AddImpulse(ThrowDirection * ThrowForce, NAME_None, true);
	}
}

