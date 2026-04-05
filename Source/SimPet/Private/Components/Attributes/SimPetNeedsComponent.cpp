// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/Attributes/SimPetNeedsComponent.h"

#include "SimPetDebugHelper.h"

USimPetNeedsComponent::USimPetNeedsComponent()
{
	SecondsPerGameHour = 1.0f;
	
	bNeedsCleaning = false;
	bNeedsFeed = false;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;

	HungryThresholdHours = 8.0f;
	DeathThresholdHours = 24.0f;
	DirtyThresholdHours = 12.0f;
}

void USimPetNeedsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StartNeedsTimer();
}

void USimPetNeedsComponent::DisableNeedsAndBecomeHappy()
{
	Feed();
	Wash();
	
	StopNeedsTimer();
}

void USimPetNeedsComponent::EnableNeeds()
{
	StartNeedsTimer();
}

ESimPetAnimalState USimPetNeedsComponent::GetAnimalState()
{
	return AnimalState;
}

void USimPetNeedsComponent::Feed()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;
	
	TimeSinceLastMeal = 0.0f;	
	bNeedsFeed = false;
	
	OnHungrySatisfied.Broadcast();

	TryEnterHappyState();
}

void USimPetNeedsComponent::Wash()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;
	
	TimeSinceLastClean = 0.0f;
	bNeedsCleaning = false;
	
	OnGotClean.Broadcast();
	
	TryEnterHappyState();
}

void USimPetNeedsComponent::StartNeedsTimer()
{
	GetWorld()->GetTimerManager().SetTimer(NeedsTimerHandle, this, &USimPetNeedsComponent::OnNeedsTick, SecondsPerGameHour, true);
}

void USimPetNeedsComponent::StopNeedsTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(NeedsTimerHandle);
}

void USimPetNeedsComponent::OnNeedsTick()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;

	TimeSinceLastMeal++;
	TimeSinceLastClean++;

	CheckPhysicalCharacterState();
	
	if (AnimalState == ESimPetAnimalState::Happy)
		OnHappyTick.Broadcast();
}

void USimPetNeedsComponent::CheckPhysicalCharacterState()
{
	if (TimeSinceLastMeal >= DeathThresholdHours)
	{
		Die();
		return;
	}
	
	if (!bNeedsFeed && TimeSinceLastMeal >= HungryThresholdHours)
	{
		BecomeHungry();
	}
	
	if (!bNeedsCleaning && TimeSinceLastClean >= DirtyThresholdHours)
	{
		BecomeDirty();
	}
}

void USimPetNeedsComponent::Die()
{
	EnterDeadState();
	
	StopNeedsTimer();

	OnDied.Broadcast();
}

void USimPetNeedsComponent::BecomeDirty()
{
	if (bNeedsCleaning == true)
		return;
	
	bNeedsCleaning = true;
	
	OnGotDirty.Broadcast();
	
	EnterTiredState();
}

void USimPetNeedsComponent::BecomeHungry()
{
	bNeedsFeed = true;
	
	OnGotHungry.Broadcast();
	
	EnterTiredState();
}

void USimPetNeedsComponent::TryEnterHappyState()
{
	if (bNeedsFeed == true || bNeedsCleaning == true)
		return;
	
	EnterHappyState();
}

void USimPetNeedsComponent::EnterHappyState()
{
	if (AnimalState == ESimPetAnimalState::Tired)
	{
		AnimalState = ESimPetAnimalState::Happy;
	}
}

void USimPetNeedsComponent::EnterTiredState()
{
	if (AnimalState == ESimPetAnimalState::Happy)
	{
		AnimalState = ESimPetAnimalState::Tired;
	}
}

void USimPetNeedsComponent::EnterDeadState()
{
	if (AnimalState == ESimPetAnimalState::Tired)
	{
		AnimalState = ESimPetAnimalState::Dead;
	}
}
