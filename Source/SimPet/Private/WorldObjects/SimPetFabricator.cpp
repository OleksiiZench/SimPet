// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"

#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
	Debug::Print(__func__);

	if (TSubclassOf<ASimPetAnimal> *FoundClassAnimal = AnimalClassMap.Find(CurrentAnimal))
	{
		FTransform AnimalTransform = FTransform(FRotator::ZeroRotator, FVector3d(50.0f, 50.0f, 400.0f));
		
		GetWorld()->SpawnActor<ASimPetAnimal>(*FoundClassAnimal, AnimalTransform);
			
		Debug::Print(TEXT("Spawned: ") + (*FoundClassAnimal)->GetName());
	}
}
