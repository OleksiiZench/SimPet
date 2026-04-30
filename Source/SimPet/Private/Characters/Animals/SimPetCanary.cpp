// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetCanary.h"

#include "GameFramework/CharacterMovementComponent.h"

ASimPetCanary::ASimPetCanary()
{
	MovementComponent = GetCharacterMovement();
	
	MovementComponent->BrakingDecelerationFlying = 2000.0f;
	MovementComponent->MaxFlySpeed = 600.0f;
	
	WingsLiftOffset = 15.0f;
}

void ASimPetCanary::BeginPlay()
{
	Super::BeginPlay();
}

void ASimPetCanary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 1. Анімація крил
	AnimateWings(DeltaTime);
}

void ASimPetCanary::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	if (SpawnedWings)
	{
		SpawnedWings->DestroyComponent();
		SpawnedWings = nullptr;
	}

	if (bHasWings && WingsAsset && !WingsSocket.IsNone())
	{
		UStaticMeshComponent *NewComp = NewObject<UStaticMeshComponent>(this);
		NewComp->SetStaticMesh(WingsAsset);
		NewComp->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, WingsSocket);
		NewComp->RegisterComponent();

		SpawnedWings = NewComp;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetCanary::SetFlyingMovementMode(const bool IsFlying)
{
	if (IsFlying)
	{
		MovementComponent->SetMovementMode(MOVE_Flying);
	}
	else
	{
		MovementComponent->SetMovementMode(MOVE_Walking);  // MOVE_Walking перевірити чи це працивильний режим не польоту!!!!!
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetCanary::AnimateWings(float DeltaTime)
{
	constexpr float ReturnSpeed = 10.0f;  // Швидкість повернення крил у вихідне положення
	
	FVector CurrentLocation = SpawnedWings->GetRelativeLocation();
	
	float TargetZ = 0.0f;
	
	if (MovementComponent->IsFlying() || MovementComponent->IsFalling())
	{
		constexpr float FlySpeed = 15.0f;  // частота махів крил
		
		TargetZ = FMath::Sin(GetGameTimeSinceCreation() * FlySpeed) * WingsLiftOffset;
	}
	
	float NewZ = FMath::FInterpTo(CurrentLocation.Z, TargetZ, DeltaTime, ReturnSpeed);
	
	SpawnedWings->SetRelativeLocation(FVector(CurrentLocation.X, CurrentLocation.Y, NewZ));
}
