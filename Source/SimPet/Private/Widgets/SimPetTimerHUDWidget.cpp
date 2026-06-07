// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetTimerHUDWidget.h"

#include "Components/TextBlock.h"

#include "Subsystems/SimPetTimeSubsystem.h"

void USimPetTimerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CacheTimeSubsystem();
	
	BindDelegate();
	
	if (CachedTimeSubsystem)
		UpdateTimerDisplay(CachedTimeSubsystem->GetCurrentPlayTime());
}

void USimPetTimerHUDWidget::NativeDestruct()
{
	UnbindDelegate();
	
	Super::NativeDestruct();
}

void USimPetTimerHUDWidget::UpdateTimerDisplay(int32 CurrentTimeSeconds)
{
	if (TimerText)
	{
		// Форматуємо час у mm:ss
		FTimespan Timespan = FTimespan::FromSeconds(CurrentTimeSeconds);
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Timespan.GetMinutes(), Timespan.GetSeconds());
		
		TimerText->SetText(FText::FromString(TimeString));
	}
}

void USimPetTimerHUDWidget::CacheTimeSubsystem()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return;
	
	CachedTimeSubsystem = World->GetSubsystem<USimPetTimeSubsystem>();
}

void USimPetTimerHUDWidget::BindDelegate()
{
	if (CachedTimeSubsystem == nullptr)
		return;
	
	CachedTimeSubsystem->OnTimeUpdated.AddDynamic(this, &ThisClass::UpdateTimerDisplay);
}

void USimPetTimerHUDWidget::UnbindDelegate()
{
	if (CachedTimeSubsystem == nullptr)
		return;
	
	CachedTimeSubsystem->OnTimeUpdated.RemoveDynamic(this, &ThisClass::UpdateTimerDisplay);
}
