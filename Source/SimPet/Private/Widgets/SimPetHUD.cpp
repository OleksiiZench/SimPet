// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUD.h"

#include "Blueprint/UserWidget.h"

#include "Subsystems/SimPetUISubsystem.h"
#include "Widgets/SimPetNotificationWidget.h"

#include "SimPetDebugHelper.h"

void ASimPetHUD::BeginPlay()
{
	Super::BeginPlay();
	
	CreateWidgets();
	
	CacheUISubsystem();
	
	if (CachedUISubsystem != nullptr)
	{
		CachedUISubsystem->OnShowUINotification.AddDynamic(this, &ThisClass::HandleShowNotification);
	}
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

void ASimPetHUD::CreateWidgets()
{
	CurrentGameplayHUDWidget = CreateAndAddToViewportWidget(GameplayHUDWidgetClass);
	CurrentTimerHUDWidget = CreateAndAddToViewportWidget(TimerHUDWidgetClass, 5);
	
	if (NotificationWidgetClass)
	{
		CurrentNotificationWidget = CreateWidget<USimPetNotificationWidget>(GetOwningPlayerController(), NotificationWidgetClass);
		if (CurrentNotificationWidget)
		{
			CurrentNotificationWidget->AddToViewport(10);
			
			CurrentNotificationWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASimPetHUD::CacheUISubsystem()
{
	if (GetWorld())
	{
		CachedUISubsystem = GetWorld()->GetSubsystem<USimPetUISubsystem>();
	}
}

void ASimPetHUD::HandleShowNotification(bool bIsSuccess, const FString &Message)
{
	Debug::Print(TEXT("HandleShowNotification"));
	
	if (CurrentNotificationWidget)
	{
		CurrentNotificationWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		CurrentNotificationWidget->ShowNotification(bIsSuccess, Message);
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
