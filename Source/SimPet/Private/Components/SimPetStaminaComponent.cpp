// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetStaminaComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/SimPetPlayer.h"

USimPetStaminaComponent::USimPetStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	WalkSpeed = 500.0f;
	SprintSpeed = 800.0f;
	
	MaxStamina = 3.0f;
	StaminaDrainRate = 1.0f;
	StaminaRegenRate = 1.0f;
	CurrentStamina = MaxStamina;
}

void USimPetStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CachePlayerAndCharacterMovementComp();
	
	if (CharacterMovementComp)
		CharacterMovementComp->MaxWalkSpeed = WalkSpeed;
}

void USimPetStaminaComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateStamina(DeltaTime);
}

bool USimPetStaminaComponent::IsMovingHorizontally()
{
	if (!Player)
		return false;
	
	return !(Player->GetVelocity().Size2D() < 5.0f);
}

void USimPetStaminaComponent::UpdateStamina(float DeltaTime)
{
	if (!CharacterMovementComp)
		return;
	
	float OldStamina = CurrentStamina;
	
	if (bIsSprinting)
	{
		bool bIsFalling = CharacterMovementComp->IsFalling();
		
		if (!IsMovingHorizontally() || bIsFalling)
		{
			SetSprint(false);
		}
		else
		{
			CurrentStamina -= StaminaDrainRate * DeltaTime;
		
			if (CurrentStamina <= 0.0f)
			{
				CurrentStamina = 0.0f;
				SetSprint(false);
			}
		}
	}
	else if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * DeltaTime;
	}
	
	if (!FMath::IsNearlyEqual(OldStamina, CurrentStamina))
	{
		float StaminaPercent = FMath::Clamp(CurrentStamina / MaxStamina, 0.0f, 1.0f);
		
		if (OnStaminaChanged.IsBound())
			OnStaminaChanged.Broadcast(StaminaPercent);
	}
}

void USimPetStaminaComponent::CachePlayerAndCharacterMovementComp()
{
	if (ASimPetPlayer *TempPlayer = Cast<ASimPetPlayer>(GetOwner()))
	{
		Player = TempPlayer;
		
		CharacterMovementComp = Player->GetCharacterMovement();
	}
}

void USimPetStaminaComponent::SetSprint(bool bIsSprint)
{
	if (!CharacterMovementComp)
		return;
	
	if (bIsSprint && IsMovingHorizontally())
	{
		CharacterMovementComp->MaxWalkSpeed = SprintSpeed;
		bIsSprinting = true;
	}
	else
	{
		CharacterMovementComp->MaxWalkSpeed = WalkSpeed;
		bIsSprinting = false;
	}
}
