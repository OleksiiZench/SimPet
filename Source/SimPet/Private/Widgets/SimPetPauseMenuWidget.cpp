// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetPauseMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/SimPetPlayer.h"
#include "Controllers/SimPetPlayerController.h"
#include "Subsystems/SimPetSaveSubsystem.h"
#include "Core/SimPetPlayerState.h"

void USimPetPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CachePlayerController();
	CacheSaveSubsystem();
	CachePlayerState();
	
	SetupButtonBindings();
	
	UpdateAssignedText();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::OnResumeClicked()
{
	if (CachedPlayerController)
		CachedPlayerController->TogglePauseMenu();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::OnToMainMenuClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}

void USimPetPauseMenuWidget::SetupButtonBindings()
{
	if (Btn_Resume)
		Btn_Resume->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnResumeClicked);
	
	if (Btn_ToMainMenu)
		Btn_ToMainMenu->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnToMainMenuClicked);
}

void USimPetPauseMenuWidget::UpdateAssignedText()
{
	UpdateMaxPointsText();
	UpdateCurrentPointsText();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateMaxPointsText()
{
	if (MaxPointsText)
	{
		int32 MaxPoints = CachedSaveSubsystem->GetMaxPoints();
		
		MaxPointsText->SetText(FText::AsNumber(MaxPoints));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateCurrentPointsText()
{
	if (CurrentPointsText)
	{
		int32 CurrentPoints = CachedPlayerState->GetCurrentPoints();
		
		CurrentPointsText->SetText(FText::AsNumber(CurrentPoints));
	}
}

void USimPetPauseMenuWidget::CachePlayerController()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
		CachedPlayerController = Cast<ASimPetPlayerController>(PlayerCharacter->GetController());
}

void USimPetPauseMenuWidget::CacheSaveSubsystem()
{
	if (GetWorld())
	{
		CachedSaveSubsystem = GetWorld()->GetSubsystem<USimPetSaveSubsystem>();
	}
}

void USimPetPauseMenuWidget::CachePlayerState()
{
	if (Cast<ASimPetPlayerState>(GetOwningPlayerState()))
	{
		CachedPlayerState = Cast<ASimPetPlayerState>(GetOwningPlayerState());
	}
}
