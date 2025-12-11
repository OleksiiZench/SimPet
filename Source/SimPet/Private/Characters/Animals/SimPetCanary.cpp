// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetCanary.h"
#include "GameFramework/CharacterMovementComponent.h"

ASimPetCanary::ASimPetCanary()
{
	GetCharacterMovement()->BrakingDecelerationFlying = 2000.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;
}

void ASimPetCanary::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
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
