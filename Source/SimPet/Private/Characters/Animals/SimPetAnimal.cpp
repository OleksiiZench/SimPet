// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetAnimal.h"

#include "Components/CapsuleComponent.h"

#include "SimPetDebugHelper.h"

ASimPetAnimal::ASimPetAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

	// State
	bIsClean = true;
	MealPerDay = 0;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;

	// Config
	GameSpeed = 8.0f;
	TiredThresholdHours = 8.0f;
	DeathThresholdHours = 24.0f;
	DirtyThresholdHours = 24.0f;

	// Appearance
	EyesCount = 0;
	PawsCount = 0;

	TimeAccumulator = 0.0f;
}

void ASimPetAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AnimalState == ESimPetAnimalState::Dead)
		return;

	TimeAccumulator += DeltaTime;

	if (TimeAccumulator >= GameSpeed)
	{
		TimeSinceLastMeal++;
		TimeSinceLastClean++;

		TimeAccumulator -= GameSpeed;

		if (bIsClean && TimeSinceLastClean >= DirtyThresholdHours)
			ToDirty();

		if (TimeSinceLastMeal >= DeathThresholdHours)
		{
			ToDie();
			return;
		}

		UpdateAnimalState();
	}
}

void ASimPetAnimal::ToEat()
{
	TimeSinceLastMeal = 0.0f;
	MealPerDay++;

	Debug::Print("Animal ate.");

	UpdateAnimalState();
}

void ASimPetAnimal::ToClean()
{
	TimeSinceLastClean = 0.0f;
	bIsClean = true;

	Debug::Print("Animal is clean now!");

	UpdateAnimalState();
}

void ASimPetAnimal::ToDie()
{
	AnimalState = ESimPetAnimalState::Dead;

	Debug::Print(TEXT("The animal died"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ASimPetAnimal::ToDirty()
{
	bIsClean = false;

	Debug::Print(TEXT("The animal got dirty"));

	UpdateAnimalState();
}

void ASimPetAnimal::UpdateAnimalState()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;

	if (AnimalState != ESimPetAnimalState::Happy && TimeSinceLastMeal < TiredThresholdHours && bIsClean == true)
	{
		AnimalState = ESimPetAnimalState::Happy;
		Debug::Print("Animal state changed to Happy");
	}
	else if (AnimalState != ESimPetAnimalState::Tired && ((TimeSinceLastMeal >= TiredThresholdHours && TimeSinceLastMeal < DeathThresholdHours) || !bIsClean))
	{
		AnimalState = ESimPetAnimalState::Tired;
		Debug::Print("Animal state changed to Tired");
	}
}
