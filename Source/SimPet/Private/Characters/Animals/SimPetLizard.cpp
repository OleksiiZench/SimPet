// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetLizard.h"

#include "Components/CapsuleComponent.h"

ASimPetLizard::ASimPetLizard()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 20.0f);
	
	LegLiftAngle = 30.0f;
}

void ASimPetLizard::AnimateLegs(float DeltaTime, float CurrentTime)
{// Повторення логіки з SimPetAnimal, але Yaw замість Pitch
	float CurrentVelocity = GetVelocity().Size();  // Розкоментувати якщо реалізований рух тварин
	
	float WalkSpeed = 15.0f;  // Швидкість ходьби, частота кроків
	float ReturnSpeed = 15.0f;  // Швидкість повернення ніг у вихідне положення
	
	for (int32 i = 0; i < SpawnedLegs.Num(); i++)
	{
		UStaticMeshComponent *Leg = SpawnedLegs[i];
		if (!Leg)
			continue;
		
		FRotator LegRotation = Leg->GetRelativeRotation();
		
		float TargetYaw = 0.0f;
		
		if (CurrentVelocity > 0.0f)
			TargetYaw = FMath::Sin((CurrentTime * WalkSpeed) + 0.0f) * LegLiftAngle;
		
		float NewYaw = FMath::FInterpTo(LegRotation.Yaw, TargetYaw, DeltaTime, ReturnSpeed);
		
		Leg->SetRelativeRotation(FRotator(LegRotation.Pitch, NewYaw, LegRotation.Roll));
	}
}
