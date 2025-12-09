// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetCanary.h"
#include "GameFramework/CharacterMovementComponent.h"

ASimPetCanary::ASimPetCanary()
{
	MovementComponent = GetCharacterMovement();
	MovementComponent->BrakingDecelerationFlying = 2000.0f;
	MovementComponent->MaxFlySpeed = 600.0f;
}

void ASimPetCanary::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->SetMovementMode(MOVE_Flying);
}
