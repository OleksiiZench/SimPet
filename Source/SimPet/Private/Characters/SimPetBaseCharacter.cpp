// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetBaseCharacter.h"

#include "Components/CapsuleComponent.h"

ASimPetBaseCharacter::ASimPetBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
}
