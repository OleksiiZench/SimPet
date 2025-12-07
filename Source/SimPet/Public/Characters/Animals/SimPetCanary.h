// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetCanary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetCanary : public ASimPetAnimal
{
	GENERATED_BODY()

public:
	ASimPetCanary();

	virtual void BeginPlay() override;

private:
	UCharacterMovementComponent *MovementComponent;
};
