// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"

ASimPetAnimal::ASimPetAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

	// State
	TimeSinceLastMeal = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;

	// Config
	GameSpeed = 8;
	TiredThresholdHours = 8.0f;
	DeathThresholdHours = 24.0f;

	TimeAccumulator = 0;
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
		TimeAccumulator -= GameSpeed;
	}

	if (AnimalState == ESimPetAnimalState::Happy && TimeSinceLastMeal >= TiredThresholdHours)
	{
		AnimalState = ESimPetAnimalState::Tired;

		Debug::Print(TEXT("The animal is tired"));
	}
	else if (AnimalState == ESimPetAnimalState::Tired && TimeSinceLastMeal >= DeathThresholdHours)
	{
		AnimalState = ESimPetAnimalState::Dead;

		Debug::Print(TEXT("The animal died"));
	}
}
