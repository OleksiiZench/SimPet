// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetCameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/SimPetPlayer.h"

USimPetCameraComponent::USimPetCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	
	
	BobVerticalAmplitude = 10.0f;
	BobHorizontalAmplitude = 5.0f;
	BobFrequencyMultiplier = 0.015f;
	BobTimer = 0.0f;
}

void USimPetCameraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CachePlayerAndCharacterMovementComp();
	DefaultCameraRelativeLocation = GetRelativeLocation();
}

void USimPetCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateCameraBob(DeltaTime);
}

void USimPetCameraComponent::UpdateCameraBob(float DeltaTime)
{
	if (!Player || !CharacterMovementComp)
		return;
	
	FVector Velocity = Player->GetVelocity();
	float Speed = Velocity.Size();
	
	bool bIsMovingOnGround = Speed > 10.0f && CharacterMovementComp->IsMovingOnGround();
	if (bIsMovingOnGround)
	{
		BobTimer += Speed *DeltaTime * BobFrequencyMultiplier;  // Збільшуємо таймер залежно від швидкості
		
		float ZOffset = FMath::Sin(BobTimer * 2.0f) * BobVerticalAmplitude;  // Вертикальний зсув
		
		float YOffset = FMath::Cos(BobTimer) * BobHorizontalAmplitude;  // Горизонтальний зсув
		
		// Створюємо нову відносну позицію
		FVector NewBobLocation = DefaultCameraRelativeLocation;
		NewBobLocation.Z += ZOffset;
		NewBobLocation.Y += YOffset;
		
		// Застосовуємо нову позицію
		FVector SmoothLocation = FMath::VInterpTo(GetRelativeLocation(), NewBobLocation, DeltaTime, 10.0f);
		SetRelativeLocation(SmoothLocation);
	}
	else
	{// Коли зупинились
		FVector SmoothReturn = FMath::VInterpTo(GetRelativeLocation(), DefaultCameraRelativeLocation, DeltaTime, 5.0f);
	
		if (FVector::DistSquared(SmoothReturn, DefaultCameraRelativeLocation) < 1.0f)
		{
			SetRelativeLocation(DefaultCameraRelativeLocation);
		}
		else
		{
			SetRelativeLocation(SmoothReturn);
		}
	}
}

void USimPetCameraComponent::CachePlayerAndCharacterMovementComp()
{
	if (ASimPetPlayer *TempPlayer = Cast<ASimPetPlayer>(GetOwner()))
	{
		Player = TempPlayer;
		CharacterMovementComp = Player->GetCharacterMovement();
	}
}
