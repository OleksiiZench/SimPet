// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Items/SimPetItem.h"

#include "SimPetBackPack.generated.h"

class ASimPetItem;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetBackPack : public ASimPetItem
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void SecondaryInteract_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	virtual bool TryInteractWithAnotherItem(ASimPetItem *TargetItem) override;
	
	bool TryAddItem(ASimPetItem *Item);
	void ExtractItem();
	
	UPROPERTY(EditAnywhere, Category = "Backpack")
	int32 MaxCapacity = 4;
	
private:
	void HideAndOptimizeItem(ASimPetItem *Item);
	void AttachItemToBackpack(ASimPetItem *Item);
	
	UPROPERTY()
	TArray<ASimPetItem *> StoredItems;
};
