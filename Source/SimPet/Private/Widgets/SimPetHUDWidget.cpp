// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUDWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Characters/SimPetPlayer.h"
#include "Components/Attributes/SimPetStaminaComponent.h"
#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "SimPetTypes/SimPetEnumTypes.h"

void USimPetHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CacheDependencies();
	CacheAnimalSubsystem();
	SetupInitialValues();
	SetupUIBindings();
	
	UpdateAnimalStats();
	
	BindDelegates();
}

void USimPetHUDWidget::NativeDestruct()
{
	UnbindDelegates();
	
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
	FString FormattedString = FString::Printf(TEXT("%d"), Points);
	
	if (Text_TotalPoints)
	{
		Text_TotalPoints->SetText(FText::FromString(FormattedString));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetHUDWidget::UpdateAnimalStats()
{
	if (!GetWorld())
		return;
	
	if (!CachedAnimalSubsystem)
		return;
	
	if (DogCountText)
	{
		int32 NumberOfDogs = CachedAnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Dog);
		
		DogCountText->SetText(FText::AsNumber(NumberOfDogs));
	}
	
	if (CanaryCountText)
	{
		int32 NumberOfCanaries = CachedAnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Canary);
		
		CanaryCountText->SetText(FText::AsNumber(NumberOfCanaries));
	}
	
	if (LizardCountText)
	{
		int32 NumberOfLizards = CachedAnimalSubsystem->GetNumberOwnerAnimalsCertainType(ESimPetAnimals::EA_Lizard);
		
		LizardCountText->SetText(FText::AsNumber(NumberOfLizards));
	}
	
	if (WildAnimalCountText)
	{
		int32 NumberOfWildAnimals = CachedAnimalSubsystem->GetTotalNumberWildAnimals();
		
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

void USimPetHUDWidget::CacheAnimalSubsystem()
{
	if (GetWorld())
		CachedAnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
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

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetHUDWidget::SetupUIBindings()
{
	if (Text_Points)
		Text_Points->SetText(NSLOCTEXT("HUD", "Text_Points", "Points: "));
	
	if (Text_OwnerAnimals)
		Text_OwnerAnimals->SetText(NSLOCTEXT("HUD", "Text_OwnerAnimals", "The owner's animals"));
	
	if (Text_Dog)
		Text_Dog->SetText(NSLOCTEXT("HUD", "Text_Dog", "Dogs:"));
	
	if (Text_Canary)
		Text_Canary->SetText(NSLOCTEXT("HUD", "Text_Canary", "Canaries:"));
	
	if (Text_Lizard)
		Text_Lizard->SetText(NSLOCTEXT("HUD", "Text_Lizard", "Lizards:"));
	
	if (Text_WildAnimals)
		Text_WildAnimals->SetText(NSLOCTEXT("HUD", "Text_WildAnimals", "Animals in the wild"));
	
	if (Text_Animals)
		Text_Animals->SetText(NSLOCTEXT("HUD", "Text_Animals", "Animals:"));
}

void USimPetHUDWidget::BindDelegates()
{
	if (CachedPlayerStaminaComponent)
		CachedPlayerStaminaComponent->OnStaminaChanged.AddDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
	
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.AddDynamic(this, &USimPetHUDWidget::UpdatePointsText);
	
	if (CachedAnimalSubsystem)
		CachedAnimalSubsystem->OnAnimalCountChanged.AddDynamic(this, &USimPetHUDWidget::UpdateAnimalStats);
}

void USimPetHUDWidget::UnbindDelegates()
{
	if (CachedPlayerStaminaComponent)
		CachedPlayerStaminaComponent->OnStaminaChanged.RemoveDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
	
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.RemoveDynamic(this, &USimPetHUDWidget::UpdatePointsText);
	
	if (CachedAnimalSubsystem)
		CachedAnimalSubsystem->OnAnimalCountChanged.RemoveDynamic(this, &USimPetHUDWidget::UpdateAnimalStats);
}
