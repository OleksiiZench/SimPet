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

	// BodyParts
	EyesCount = 0;
	LegsCount = 0;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetRootComponent());

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(BodyMesh);

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	TailMesh->SetupAttachment(BodyMesh);

	// Other
	TimeAccumulator = 0.0f;
}

void ASimPetAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Симуляція метаболізму та життєвого циклу
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
	
	// 2. Рух ніг
	if (true /*GetVelocity().Size() > 0.0f*/)
	{
		AnimateLegs(DeltaTime, GetGameTimeSinceCreation());
	}
}

void ASimPetAnimal::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	for (UStaticMeshComponent *SpawnedEye : SpawnedEyes)
	{
		if (SpawnedEye)
			SpawnedEye->DestroyComponent();

		SpawnedEye = nullptr;
	}
	SpawnedEyes.Empty();

	for (UStaticMeshComponent *SpawnedLeg : SpawnedLegs)
	{
		if (SpawnedLeg)
			SpawnedLeg->DestroyComponent();

		SpawnedLeg = nullptr;
	}
	SpawnedLegs.Empty();

	if (EyesAsset && EyesSockets.Num() > 0)
	{
		int32 NumEyesToSpawn = FMath::Min(EyesCount, EyesSockets.Num());

		for (int i = 0; i < NumEyesToSpawn; i++)
		{
			UStaticMeshComponent *NewComp = NewObject<UStaticMeshComponent>(this);
			NewComp->SetStaticMesh(EyesAsset);

			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			NewComp->AttachToComponent(HeadMesh, FAttachmentTransformRules::KeepRelativeTransform, EyesSockets[i]);
			NewComp->RegisterComponent();

			SpawnedEyes.Add(NewComp);
		}
	}

	if (LegsAsset && LegsSockets.Num() > 0)
	{
		int32 NumLegsToSpawn = FMath::Min(LegsCount, LegsSockets.Num());

		for (int i = 0; i < NumLegsToSpawn; i++)
		{
			UStaticMeshComponent *NewComp = NewObject<UStaticMeshComponent>(this);
			NewComp->SetStaticMesh(LegsAsset);

			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			NewComp->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, LegsSockets[i]);
			NewComp->RegisterComponent();

			SpawnedLegs.Add(NewComp);
		}
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

void ASimPetAnimal::AnimateLegs(float DeltaTime, float CurrentTime)
{
	float WalkSpeed = 10.0f;  // Швидкість ходьби, частота кроків
	float Time = CurrentTime;
	
	for (int32 i = 0; i < SpawnedLegs.Num(); i++)
	{
		UStaticMeshComponent *Leg = SpawnedLegs[i];
		if (!Leg)
			continue;
			
		FString LegSocketName = LegsSockets[i].ToString();
	 	
		bool bIsRightLeg = LegSocketName.Contains(TEXT("Right"));
		float PhaseOffset = bIsRightLeg ? UE_PI : 0.0f;
			
		float NewPitch = FMath::Sin((Time * WalkSpeed) + PhaseOffset) * LegLiftAngle;
			
		FRotator LegRotation = Leg->GetRelativeRotation();
		Leg->SetRelativeRotation(FRotator(NewPitch, LegRotation.Yaw, LegRotation.Roll));
	}
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
