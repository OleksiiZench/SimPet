// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"

#include "Interfaces/SimPetInteractable.h"

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
	
	void EnablePhysics();
	void DisablePhysics();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
private:
	UStaticMeshComponent *CacheStaticMeshComp;
};
