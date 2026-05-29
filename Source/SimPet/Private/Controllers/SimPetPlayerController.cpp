// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Controllers/SimPetPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"

#include "SimPetGameplayTags.h"
#include "DataAsset/Input/SimPetInputConfig.h"
#include "Widgets/SimPetHUD.h"
#include "Widgets/SimPetPauseMenuWidget.h"

#include "SimPetDebugHelper.h"

void ASimPetPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	SetInputMode_Game();
}

void ASimPetPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		const UInputAction *PauseAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Pause);
		if (PauseAction)
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASimPetPlayerController::Input_Pause);
		
		const UInputAction *SaveGameAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_SaveGame);
		if (SaveGameAction)
			EnhancedInputComponent->BindAction(SaveGameAction, ETriggerEvent::Started, this, &ASimPetPlayerController::Input_SaveGame);
	}
}

void ASimPetPlayerController::SetInputMode_Game()
{
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void ASimPetPlayerController::SetInputMode_UI(UUserWidget *FocusWidget)
{
	bShowMouseCursor = true;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	if (FocusWidget)
	{
		InputMode.SetWidgetToFocus(FocusWidget->TakeWidget().ToSharedPtr());
	}
	
	SetInputMode(InputMode);
}

void ASimPetPlayerController::TogglePauseMenu()
{	
	bool bIsCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
	
	bool bNewPauseState = !bIsCurrentlyPaused;
	UGameplayStatics::SetGamePaused(GetWorld(), bNewPauseState);
	
	// Логіка відображення
	if (bNewPauseState)
	{
		if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(GetHUD()))
			CurrentHUD->SetAllHUDVisibility(false);
		
		if (!CurrentPauseMenuWidget && PauseMenuWidgetClass)
		{
			CurrentPauseMenuWidget = CreateWidget<USimPetPauseMenuWidget>(this, PauseMenuWidgetClass);
		}
		
		if (CurrentPauseMenuWidget && !CurrentPauseMenuWidget->IsInViewport())
		{
			CurrentPauseMenuWidget->AddToViewport(10);
		}
		
		SetInputMode_UI(CurrentPauseMenuWidget);
	}
	else
	{
		if (CurrentPauseMenuWidget)
		{
			CurrentPauseMenuWidget->RemoveFromParent();
		}
		
		if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(GetHUD()))
			CurrentHUD->SetAllHUDVisibility(true);
		
		SetInputMode_Game();
	}
}

void ASimPetPlayerController::Input_Pause(const FInputActionValue &InputActionValue)
{
	TogglePauseMenu();
}

void ASimPetPlayerController::Input_SaveGame(const FInputActionValue &InputActionValue)
{
	Debug::Print(TEXT("Input_SaveGame"));
}
