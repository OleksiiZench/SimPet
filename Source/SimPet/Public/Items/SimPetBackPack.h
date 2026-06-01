// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Items/SimPetItem.h"
#include "Interfaces/SimPetItemContainer.h"

#include "SimPetBackPack.generated.h"

class ASimPetItem;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetBackPack : public ASimPetItem, public ISimPetItemContainer
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void SecondaryInteract_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	//~ Begin ISimPetItemContainer Interface
	virtual bool TryAddItemToContainer_Implementation(ASimPetItem *Item) override;
	virtual void GetContainerPayloadClasses_Implementation(TArray<TSubclassOf<ASimPetItem>> &OutPayloadClasses) override;
	//~ End ISimPetItemContainer Interface
	
	virtual bool TryInteractWithAnotherActor(AActor *TargetActor) override;
	
	bool TryAddItem(ASimPetItem *Item);
	void ExtractItem();
	
	UPROPERTY(EditAnywhere, Category = "Backpack")
	int32 MaxCapacity = 4;
	
	UPROPERTY(VisibleAnywhere, Category = "Backpack")
	TArray<ASimPetItem *> StoredItems;
	
private:
	void HideAndOptimizeItem(ASimPetItem *Item);
	void AttachItemToBackpack(ASimPetItem *Item);
	void ShowAndDetachItem(ASimPetItem *Item);
	void EnablePhysicsAndApplyImpulse(ASimPetItem *Item);
	
	
};
