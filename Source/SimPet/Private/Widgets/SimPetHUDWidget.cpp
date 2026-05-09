// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Characters/SimPetPlayer.h"
#include "Components/Attributes/SimPetStaminaComponent.h"
#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "SimPetEnumTypes.h"

void USimPetHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CacheDependencies();
	SetupInitialValues();
	
	InitializeTimer();
	UpdateAnimalStats();
	
	BindDelegates();
}

void USimPetHUDWidget::NativeDestruct()
{
	UnbindDelegates();
	ClearTimer();
	
	Super::NativeDestruct();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetHUDWidget::UpdateStaminaBar(float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetHUDWidget::UpdatePointsText(int32 Points)
{
	FString FormattedString = FString::Printf(TEXT("Points: %d"), Points);
	
	if (PointsText)
	{
		PointsText->SetText(FText::FromString(FormattedString));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetHUDWidget::UpdateAnimalStats()
{
	if (!GetWorld())
		return;
	
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	
	if (!AnimalSubsystem)
		return;
	
	if (DogCountText)
	{
		int32 NumberOfDogs = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Dog);
		
		DogCountText->SetText(FText::AsNumber(NumberOfDogs));
	}
	
	if (CanaryCountText)
	{
		int32 NumberOfCanaries = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Canary);
		
		CanaryCountText->SetText(FText::AsNumber(NumberOfCanaries));
	}
	
	if (LizardCountText)
	{
		int32 NumberOfLizards = AnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Lizard);
		
		LizardCountText->SetText(FText::AsNumber(NumberOfLizards));
	}
	
	if (WildAnimalCountText)
	{
		int32 NumberOfWildAnimals = AnimalSubsystem->GetTotalNumberWildAnimals();
		
		WildAnimalCountText->SetText(FText::AsNumber(NumberOfWildAnimals));
	}
}

void USimPetHUDWidget::CacheDependencies()
{
	if (ASimPetPlayer *SimPetPlayer = Cast<ASimPetPlayer>(GetOwningPlayerPawn()))
	{
		CachedPlayerStaminaComponent = SimPetPlayer->GetStaminaComponent();
		CachedPlayerState = Cast<ASimPetPlayerState>(SimPetPlayer->GetPlayerState());
	}
}

void USimPetHUDWidget::SetupInitialValues()
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(1.0f);
	}
	
	if (CachedPlayerState)
	{
		int32 CurrentPoints = CachedPlayerState->GetCurrentPoints();
		UpdatePointsText(CurrentPoints);
	}
}

void USimPetHUDWidget::InitializeTimer()
{
	if (GetWorld())
	{// Оновлення статистики раз на секунду
		GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &USimPetHUDWidget::UpdateAnimalStats, 1.0f, true);
	}
}

void USimPetHUDWidget::ClearTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(StatsTimerHandle);
	}
}

void USimPetHUDWidget::BindDelegates()
{
	if (CachedPlayerStaminaComponent)
		CachedPlayerStaminaComponent->OnStaminaChanged.AddDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
	
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.AddDynamic(this, &USimPetHUDWidget::UpdatePointsText);
}

void USimPetHUDWidget::UnbindDelegates()
{
	if (CachedPlayerStaminaComponent)
		CachedPlayerStaminaComponent->OnStaminaChanged.RemoveDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
	
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.RemoveDynamic(this, &USimPetHUDWidget::UpdatePointsText);
}
