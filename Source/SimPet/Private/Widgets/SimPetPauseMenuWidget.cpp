// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetPauseMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

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

void USimPetPauseMenuWidget::OnSaveGameClicked()
{
	Debug::Print(TEXT("OnSaveGameClicked"));
}

void USimPetPauseMenuWidget::SetupButtonBindings()
{
	if (Btn_Resume)
		Btn_Resume->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnResumeClicked);
	
	if (Btn_ToMainMenu)
		Btn_ToMainMenu->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnToMainMenuClicked);
	
	if (Btn_SaveGame)
		Btn_SaveGame->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnSaveGameClicked);
}

void USimPetPauseMenuWidget::UpdateAssignedText()
{
	UpdateMaxPointsText();
	UpdateCurrentPointsText();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateMaxPointsText()
{
	if (MaxPointsText && CachedSaveSubsystem)
	{
		int32 MaxPoints = CachedSaveSubsystem->GetMaxPoints();
		
		MaxPointsText->SetText(FText::AsNumber(MaxPoints));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPauseMenuWidget::UpdateCurrentPointsText()
{
	if (CurrentPointsText && CachedPlayerState)
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
