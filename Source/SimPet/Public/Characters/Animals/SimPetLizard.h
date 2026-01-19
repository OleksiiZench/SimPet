// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetLizard.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetLizard : public ASimPetAnimal
{
	GENERATED_BODY()
	
public:
	ASimPetLizard();
	
protected:
	virtual void AnimateLegs(float DeltaTime, float CurrentTime) override;
};
