// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalWaste.h"

#include "Subsystems/SimPetAnimalSubsystem.h"

void ASimPetAnimalWaste::Interact_Implementation(AActor *InstigatorActor)
{
	CleanNearestAnimal();
	
	OnWasteCleaned.Broadcast();
	
	Destroy();
}

void ASimPetAnimalWaste::CleanNearestAnimal()
{
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem)
		AnimalSubsystem->HandleWasteCleaned();
}
