// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Characters/SimPetPlayer.h"
#include "Components/Attributes/SimPetStaminaComponent.h"
#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetAnimalSubsystem.h"

void USimPetHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GetWorld())
	{// Оновлення статистики раз на секунду
		GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &USimPetHUDWidget::UpdateAnimalStats, 1.0f, true);
	}
	
	UpdateAnimalStats();
	
	if (ASimPetPlayer *SimPetPlayer = Cast<ASimPetPlayer>(GetOwningPlayerPawn()))
	{
		StaminaProgressBar->SetPercent(1.0f);
		
		if (USimPetStaminaComponent *PlayerStaminaComponent = SimPetPlayer->GetStaminaComponent())
		{
			PlayerStaminaComponent->OnStaminaChanged.AddDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
		}
		
		if (ASimPetPlayerState *PlayerState = Cast<ASimPetPlayerState>(SimPetPlayer->GetPlayerState()))
		{
			int32 CurrentPoints = PlayerState->GetCurrentPoints();
			UpdatePointsText(CurrentPoints);
			
			PlayerState->OnPointsChanged.AddDynamic(this, &USimPetHUDWidget::UpdatePointsText);
		}
	}
}

void USimPetHUDWidget::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(StatsTimerHandle);
	}
	
	Super::NativeDestruct();
}

void USimPetHUDWidget::UpdateStaminaBar(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void USimPetHUDWidget::UpdatePointsText(int32 Points)
{
	FString FormattedString = FString::Printf(TEXT("Points: %d"), Points);
	
	if (PointsText)
	{
		PointsText->SetText(FText::FromString(FormattedString));
	}
}

void USimPetHUDWidget::UpdateAnimalStats()
{
	if (!GetWorld())
		return;
	
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	
	if (!AnimalSubsystem)
		return;
	
	if (DogCountText)
	{
		int NumberOfDogs = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Dog);
		
		DogCountText->SetText(FText::AsNumber(NumberOfDogs));
	}
	
	if (CanaryCountText)
	{
		int NumberOfCanaries = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Canary);
		
		CanaryCountText->SetText(FText::AsNumber(NumberOfCanaries));
	}
	
	if (LizardCountText)
	{
		int NumberOfLizards = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Lizard);
		
		LizardCountText->SetText(FText::AsNumber(NumberOfLizards));
	}
	
	if (WildAnimalCountText)
	{
		int NumberOfWildAnimals = AnimalSubsystem->GetTotalNumberWildAnimals();
		
		WildAnimalCountText->SetText(FText::AsNumber(NumberOfWildAnimals));
	}
}
