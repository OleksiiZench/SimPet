// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalWaste.h"

void ASimPetAnimalWaste::Interact_Implementation(AActor *InstigatorActor)
{
	OnWasteCleaned.Broadcast();
	
	Destroy();
}
