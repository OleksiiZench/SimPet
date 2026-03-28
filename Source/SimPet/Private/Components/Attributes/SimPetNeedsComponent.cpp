// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/Attributes/SimPetNeedsComponent.h"

#include "SimPetDebugHelper.h"

USimPetNeedsComponent::USimPetNeedsComponent()
{
	SecondsPerGameHour = 0.5f;
	
	bNeedsCleaning = false;
	bNeedsFeed = false;
	MealPerDay = 0;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;

	HungryThresholdHours = 8.0f;
	DeathThresholdHours = 24.0f;
	DirtyThresholdHours = 24.0f;
}

void USimPetNeedsComponent::StartNeedsTimer()
{
	GetWorld()->GetTimerManager().SetTimer(NeedsTimerHandle, this, &USimPetNeedsComponent::OnNeedsTick, SecondsPerGameHour, true);
}

void USimPetNeedsComponent::StopNeedsTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(NeedsTimerHandle);
}

ESimPetAnimalState USimPetNeedsComponent::GetAnimalState()
{
	return AnimalState;
}

void USimPetNeedsComponent::Feed()
{
	TimeSinceLastMeal = 0.0f;
	MealPerDay++;
	
	bNeedsFeed = false;

	Debug::Print("Character ate.");

	UpdateAnimalState();
}

void USimPetNeedsComponent::Wash()
{
	TimeSinceLastClean = 0.0f;
	bNeedsCleaning = false;

	Debug::Print("Character is clean now!");

	UpdateAnimalState();
}

void USimPetNeedsComponent::Die()
{
	AnimalState = ESimPetAnimalState::Dead;
	// PointsTransactionComponent->GeneratePenaltyPoints();  // - Мабуть це буде реагування на делегат OnDied в SimPetAnimal	
	StopNeedsTimer();

	OnDied.Broadcast();  // ?????????
}

void USimPetNeedsComponent::BecomeDirty()
{
	bNeedsCleaning = true;
	
	// AnimalStatusWidget->SetDirtyIconVisible(true);  // - Мабуть це буде реагування на делегат OnGotDirty в SimPetAnimal	

	OnGotDirty.Broadcast();
	
	Debug::Print(TEXT("Character got dirty"));
}

void USimPetNeedsComponent::OnMetabolismTick()
{
	if (AnimalState == ESimPetAnimalState::Dead)
	{
		return;
	}

	TimeSinceLastMeal++;
	TimeSinceLastClean++;

	CheckPhysicalAnimalState();
	UpdateAnimalState();
	
	OnHappyTick.Broadcast();
	
	// GeneratePointsIfAnimalIsHappy();  // - Мабуть це буде реагування на делегат OnGotDirty в SimPetAnimal
}

void USimPetNeedsComponent::CheckPhysicalAnimalState()
{
	if (TimeSinceLastMeal >= DeathThresholdHours)
	{
		Die();
		return;
	}
	
	if (!bNeedsCleaning && TimeSinceLastClean >= DirtyThresholdHours)
		BecomeDirty();
}

void USimPetNeedsComponent::UpdateAnimalState()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;
	
	if (TimeSinceLastMeal >= HungryThresholdHours || bNeedsCleaning)
	{
		if (AnimalState != ESimPetAnimalState::Tired)
		{
			AnimalState = ESimPetAnimalState::Tired;
			
			// if (AnimalStatusWidget)
			// 	AnimalStatusWidget->SetHungryIconVisible(true);  // - мабуть це буде варто винести в окремий метод типу 'Got Hungry' і звідти бродкастити делегат OnGotHungry що тварина проголодалася і реагувати на нього в SimPetAnimal для зміни іконок
			
			Debug::Print("Character state changed to Tired");
		}
		
		return;
	}
	
	if (AnimalState != ESimPetAnimalState::Happy)
	{
		AnimalState = ESimPetAnimalState::Happy;
		Debug::Print("Character state changed to Happy");
	}
}

void USimPetNeedsComponent::OnNeedsTick()
{
}
