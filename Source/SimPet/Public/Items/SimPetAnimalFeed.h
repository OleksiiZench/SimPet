// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SimPetItem.h"

#include "SimPetAnimalFeed.generated.h"

class ASimPetPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFeedPickedUpSignature, ASimPetAnimalFeed *, PickedFeed);

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAnimalFeed : public ASimPetItem
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	virtual bool TryInteractWithAnotherActor(AActor *TargetActor) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnFeedPickedUpSignature OnFeedPickedUp;
	
private:
	bool TryFeedAnimal(AActor *TargetActor);
};
