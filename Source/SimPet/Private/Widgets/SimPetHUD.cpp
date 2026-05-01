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

void ASimPetHUD::SetAllHUDVisibility(bool bIsVisible)
{
	SetGameplayHUDVisibility(bIsVisible);
	SetTimerHUDVisibility(bIsVisible);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetHUD::SetGameplayHUDVisibility(const bool bIsVisible)
{
	if (CurrentGameplayHUDWidget)
	{
		CurrentGameplayHUDWidget->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetHUD::SetTimerHUDVisibility(const bool bIsVisible)
{
	if (CurrentTimerHUDWidget)
	{
		CurrentTimerHUDWidget->SetVisibility(bIsVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
UUserWidget * ASimPetHUD::CreateAndAddToViewportWidget(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
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
