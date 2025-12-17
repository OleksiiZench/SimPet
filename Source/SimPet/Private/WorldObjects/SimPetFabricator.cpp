// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"

#include "Characters/Animals/SimPetAnimal.h"
#include "Characters/Animals/SimPetCanary.h"
#include "Characters/Animals/SimPetDog.h"
#include "Characters/Animals/SimPetLizard.h"

#include "SimPetDebugHelper.h"

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
	Debug::Print(__func__);

	if (!AnimalsToSpawn.IsEmpty())
	{
		int32 RandomIndexAnimal = FMath::RandRange(0, AnimalsToSpawn.Num() - 1);
		TSubclassOf<ASimPetAnimal> SelectedAnimal = AnimalsToSpawn[RandomIndexAnimal];
	
		if (SelectedAnimal)
		{
			FTransform AnimalTransform = FTransform(FRotator::ZeroRotator, FVector3d(50.0f, 50.0f, 400.0f));
		
			GetWorld()->SpawnActor<ASimPetAnimal>(SelectedAnimal, AnimalTransform);
			
			Debug::Print(TEXT("Spawned: ") + SelectedAnimal->GetName());
		}
	}
}
