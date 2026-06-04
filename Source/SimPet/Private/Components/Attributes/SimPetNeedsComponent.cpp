// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/Attributes/SimPetNeedsComponent.h"

USimPetNeedsComponent::USimPetNeedsComponent()
{
	bNeedsCleaning = false;
	bNeedsFeed = false;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;
}

void USimPetNeedsComponent::InitializeNeeds(const FSimPetNeedsConfig &InConfig)
{
	CurrentConfig = InConfig;
	
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

ESimPetAnimalState USimPetNeedsComponent::GetAnimalState() const
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

float USimPetNeedsComponent::GetTimeSinceLastMeal() const
{
	return TimeSinceLastMeal;
}

float USimPetNeedsComponent::GetTimeSinceLastClean() const
{
	return TimeSinceLastClean;
}

void USimPetNeedsComponent::RestoreNeedsState(float SavedTimeSinceLastMeal, float SavedTimeSinceLastClean)
{
	TimeSinceLastMeal = SavedTimeSinceLastMeal;
	TimeSinceLastClean = SavedTimeSinceLastClean;
	
	CheckPhysicalCharacterState();
}

void USimPetNeedsComponent::StartNeedsTimer()
{
	StopNeedsTimer();
	
	GetWorld()->GetTimerManager().SetTimer(NeedsTimerHandle, this, &USimPetNeedsComponent::OnNeedsTick, CurrentConfig.SecondsPerGameHour, true);
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
	if (TimeSinceLastMeal >= CurrentConfig.DeathThresholdHours)
	{
		Die();
		return;
	}
	
	if (!bNeedsFeed && TimeSinceLastMeal >= CurrentConfig.HungryThresholdHours)
	{
		BecomeHungry();
	}
	
	if (!bNeedsCleaning && TimeSinceLastClean >= CurrentConfig.DirtyThresholdHours)
	{
		BecomeDirty();
	}
}

void USimPetNeedsComponent::Die()
{
	EnterDeadState();
	
	StopNeedsTimer();

	OnNeedsDepleted.Broadcast();
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
	if (AnimalState != ESimPetAnimalState::Dead)
	{
		AnimalState = ESimPetAnimalState::Dead;
	}
}
