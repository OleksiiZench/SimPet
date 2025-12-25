// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetLizard.h"

#include "Components/CapsuleComponent.h"

ASimPetLizard::ASimPetLizard()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 20.0f);
	
	LegLiftAngle = 30.0f;
}

void ASimPetLizard::AnimateLegs(float DeltaTime, float CurrentTime)
{
	float WalkSpeed = 10.0f;  // Швидкість ходьби, частота кроків
	float Time = CurrentTime;
	
	for (int32 i = 0; i < SpawnedLegs.Num(); i++)
	{
		UStaticMeshComponent *Leg = SpawnedLegs[i];
		if (!Leg)
			continue;
		
		float NewYaw = FMath::Sin((Time * WalkSpeed) + 0.0f) * LegLiftAngle;
			
		FRotator LegRotation = Leg->GetRelativeRotation();
		Leg->SetRelativeRotation(FRotator(LegRotation.Pitch, NewYaw, LegRotation.Roll));
	}
}
