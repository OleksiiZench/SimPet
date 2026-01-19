// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetMenuGameMode.h"

#include "Blueprint/UserWidget.h"

void ASimPetMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainMenuWidgetClass)
	{
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		
		if (MenuWidget)
			MenuWidget->AddToViewport();
	}
	
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = true;
		PC->SetInputMode(FInputModeUIOnly());
	}
}
