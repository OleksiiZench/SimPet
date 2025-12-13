// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetLizard.h"

#include "Components/CapsuleComponent.h"

ASimPetLizard::ASimPetLizard()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 20.0f);
}
