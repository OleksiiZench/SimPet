// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"

#include "Interfaces/SimPetInteractable.h"
#include "SimPetTypes/SimPetEnumTypes.h"

#include "SimPetItem.generated.h"

UCLASS()
class SIMPET_API ASimPetItem : public AActor, public IGameplayTagAssetInterface, public ISimPetInteractable
{
	GENERATED_BODY()
	
public:	
	ASimPetItem();
	
	virtual void BeginPlay() override;
	
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	virtual void SecondaryInteract_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface

	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;
	
	virtual bool TryInteractWithAnotherActor(AActor *TargetActor);
	virtual void NotifyPickedUp();
	
	void EnablePhysics();
	void DisablePhysics();
	
	ESimPetItemState GetItemState() const;
	void SetItemState(const ESimPetItemState NewItemState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
	ESimPetItemState CurrentItemState;
	
private:
	UPROPERTY()
	UStaticMeshComponent *CacheStaticMeshComp;
};
