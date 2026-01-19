// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Controllers/SimPetPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

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
