// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "DebugTools/SimPetDebugRadius.h"

#include "DrawDebugHelpers.h"

ASimPetDebugRadius::ASimPetDebugRadius()
{
	PrimaryActorTick.bCanEverTick = true;
	
	isShow = true;
	Radius = 500.0f;
	LineColor = FColor::Green;
	LineThickness = 5.0f;
	Segments = 32;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}
void ASimPetDebugRadius::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isShow)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			Radius,
			Segments,
			LineColor,
			false,
			-1.0f,
			0,
			LineThickness,
			GetActorForwardVector(),
			GetActorRightVector(),
			false
		);
	}
}

bool ASimPetDebugRadius::ShouldTickIfViewportsOnly() const
{
	return true;
}

