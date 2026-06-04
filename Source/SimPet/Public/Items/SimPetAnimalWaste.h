// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/SimPetItem.h"

#include "SimPetAnimalWaste.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWasteCleanedSignature);

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAnimalWaste : public ASimPetItem
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	FOnWasteCleanedSignature OnWasteCleaned;
	
private:
	void CleanNearestAnimal();
};
