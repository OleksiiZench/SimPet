// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetPauseMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

#include "Characters/SimPetPlayer.h"
#include "Controllers/SimPetPlayerController.h"
#include "Subsystems/SimPetSaveSubsystem.h"
#include "Core/SimPetPlayerState.h"

#include "SimPetDebugHelper.h"

void USimPetPauseMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CachePlayerController();
	CacheSaveSubsystem();
	CachePlayerState();
	
	SetupButtonBindings();
	SetupUIBindings();
}

void USimPetPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateAssignedText();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::OnResumeClicked()
{
	if (CachedPlayerController)
	{
		CachedPlayerController->TogglePauseMenu();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::OnToMainMenuClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::OnSaveGameClicked()
{
	if (GetWorld())
	{
		USimPetSaveSubsystem *SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
		if (SaveSubsystem)
		{
			SaveSubsystem->SaveGame();
		}
	}
}

void USimPetPauseMenuWidget::OnHelpClicked()
{
	Debug::Print("Help clicked!");
}

void USimPetPauseMenuWidget::SetupButtonBindings()
{
	if (Btn_Resume)
		Btn_Resume->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnResumeClicked);
	
	if (Btn_ToMainMenu)
		Btn_ToMainMenu->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnToMainMenuClicked);
	
	if (Btn_SaveGame)
		Btn_SaveGame->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnSaveGameClicked);\

	if (Btn_Help)
		Btn_Help->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnHelpClicked);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::SetupUIBindings()
{
	if (Text_Pause)
		Text_Pause->SetText(NSLOCTEXT("Pause", "Text_Pause", "PAUSE"));
	
	if (TextBtn_Resume)
		TextBtn_Resume->SetText(NSLOCTEXT("Resume", "TextBtn_Resume", "Resume"));
	
	if (TextBtn_SaveGame)
		TextBtn_SaveGame->SetText(NSLOCTEXT("SaveGame", "TextBtn_SaveGame", "Save Game"));
	
	if (TextBtn_ToMainMenu)
		TextBtn_ToMainMenu->SetText(NSLOCTEXT("ToMainMenu", "TextBtn_ToMainMenu", "To Main Menu"));
	
	if (Text_MaxPoints)
		Text_MaxPoints->SetText(NSLOCTEXT("MaxPoints", "Text_MaxPoints", "Max points:"));
	
	if (Text_CurrentPoints)
		Text_CurrentPoints->SetText(NSLOCTEXT("CurrentPoints", "Text_CurrentPoints", "Current points:"));
}

void USimPetPauseMenuWidget::UpdateAssignedText()
{
	UpdateMaxPointsText();
	UpdateCurrentPointsText();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateMaxPointsText()
{
	if (Text_CountMaxPoints && CachedSaveSubsystem)
	{
		int32 MaxPoints = CachedSaveSubsystem->GetMaxPoints();
		
		Text_CountMaxPoints->SetText(FText::AsNumber(MaxPoints));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateCurrentPointsText()
{
	if (Text_CountCurrentPoints && CachedPlayerState)
	{
		int32 CurrentPoints = CachedPlayerState->GetCurrentPoints();
		
		Text_CountCurrentPoints->SetText(FText::AsNumber(CurrentPoints));
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
		CachedSaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	}
}

void USimPetPauseMenuWidget::CachePlayerState()
{
	CachedPlayerState = Cast<ASimPetPlayerState>(GetOwningPlayerState());
	if (CachedPlayerState == nullptr)
	{
		Debug::PrintError(TEXT("Failed to cache PlayerState!"));
	}
}
