// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUD.h"

#include "Widgets/SimPetHUDWidget.h"

void ASimPetHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (HUDWidgetClass)
	{
		if (APlayerController *PlayerController = GetOwningPlayerController())
		{
			CurrentHUDWidget = CreateWidget<USimPetHUDWidget>(PlayerController, HUDWidgetClass);
			if (CurrentHUDWidget)
				CurrentHUDWidget->AddToViewport();
		}
	}
}

void ASimPetHUD::SetHUDVisibility(bool bIsVisible)
{
	if (CurrentHUDWidget)
	{
		CurrentHUDWidget->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}
