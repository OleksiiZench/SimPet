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

void ASimPetPlayerController::Input_Pause(const FInputActionValue &InputActionValue)
{
	TogglePauseMenu();
}

void ASimPetPlayerController::TogglePauseMenu()
{	
	bIsGamePaused = !bIsGamePaused;
	
	// 1. Ставимо/ Знімаємо паузу гри
	UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);
	
	// 2. Логіка відображення
	if (bIsGamePaused)
	{
		if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(GetHUD()))
			CurrentHUD->SetGameplayHUDVisibility(false);
		
		if (!CurrentPauseMenuWidget && PauseMenuWidgetClass)
		{
			CurrentPauseMenuWidget = CreateWidget<USimPetPauseMenuWidget>(this, PauseMenuWidgetClass);
			CurrentPauseMenuWidget->AddToViewport(10);
		}
		
		if (CurrentPauseMenuWidget)
			CurrentPauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
		
		SetInputMode_UI();
	}
	else
	{
		if (CurrentPauseMenuWidget)
			CurrentPauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		
		if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(GetHUD()))
			CurrentHUD->SetGameplayHUDVisibility(true);
		
		SetInputMode_Game();
	}
}
