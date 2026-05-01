// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetTimerHUDWidget.h"

#include "Components/TextBlock.h"

void USimPetTimerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UpdateTimerDisplay();
	
	if (GetWorld())
	{// Оновлення таймера раз на секунду
		GetWorld()->GetTimerManager().SetTimer(ClockTimerHandle, this, &USimPetTimerHUDWidget::UpdateTimerDisplay, 1.0f, true);
	}
}

void USimPetTimerHUDWidget::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(ClockTimerHandle);
	}
	
	Super::NativeDestruct();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetTimerHUDWidget::UpdateTimerDisplay()
{
	if (TimerText)
	{
		float TimeSeconds = GetWorld()->GetTimeSeconds();
		
		// Форматуємо час у mm:ss
		FTimespan Timespan = FTimespan::FromSeconds(TimeSeconds);
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Timespan.GetMinutes(), Timespan.GetSeconds());
		
		TimerText->SetText(FText::FromString(TimeString));
	}
}
