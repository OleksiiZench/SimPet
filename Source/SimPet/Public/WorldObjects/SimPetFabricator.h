// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/SimPetInteractable.h"

#include "SimPetFabricator.generated.h"

UCLASS()
class SIMPET_API ASimPetFabricator : public AActor, public ISimPetInteractable
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
};
