// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUD.h"

#include "Blueprint/UserWidget.h"

#include "SimPetDebugHelper.h"

void ASimPetHUD::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentGameplayHUDWidget = CreateAndAddToViewportWidget(GameplayHUDWidgetClass);
	CurrentTimerHUDWidget = CreateAndAddToViewportWidget(TimerHUDWidgetClass, 5);
	
	Debug::Print(TEXT("CurrentGameplayHUDWidget"), CurrentGameplayHUDWidget);
	Debug::Print(TEXT("CurrentTimerHUDWidget"), CurrentTimerHUDWidget);
}

void ASimPetHUD::SetGameplayHUDVisibility(bool bIsVisible)
{
	if (CurrentGameplayHUDWidget)
	{
		CurrentGameplayHUDWidget->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

UUserWidget * ASimPetHUD::CreateAndAddToViewportWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
	if (WidgetClass == nullptr)
		return nullptr;
	
	APlayerController *PlayerController = GetOwningPlayerController();
	if (PlayerController == nullptr)
		return nullptr;
	
	UUserWidget *Widget = CreateWidget<UUserWidget>(PlayerController, WidgetClass);
	if (Widget == nullptr)
		return nullptr;
	
	Widget->AddToViewport(ZOrder);
	
	return Widget;
}
