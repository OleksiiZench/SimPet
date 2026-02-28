// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SimPetItem.h"

#include "SimPetAnimalFeed.generated.h"

class ASimPetPlayer;

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
	
private:
	void AttachToPlayer(AActor *InstigatorActor);
	ASimPetPlayer* СastInstigatorToPlayer(AActor *InstigatorActor);
};
