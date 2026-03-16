// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHUDWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ProgressBar.h"

#include "Characters/Animals/SimPetDog.h"
#include "Characters/Animals/SimPetCanary.h"
#include "Characters/Animals/SimPetLizard.h"
#include "Characters/SimPetPlayer.h"
#include "Components/SimPetStaminaComponent.h"

void USimPetHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GetWorld())
	{// Оновлення статистики раз на секунду
		GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, &USimPetHUDWidget::UpdateAnimalStats, 1.0f, true);
	}
	
	UpdateAnimalStats();
	
	if (ASimPetPlayer *SimPetPlayer = Cast<ASimPetPlayer>(GetOwningPlayerPawn()))
	{
		StaminaProgressBar->SetPercent(1.0f);
		
		if (USimPetStaminaComponent *PlayerStaminaComponent = SimPetPlayer->GetStaminaComponent())
		{
			PlayerStaminaComponent->OnStaminaChanged.AddDynamic(this, &USimPetHUDWidget::UpdateStaminaBar);
		}
	}
}

void USimPetHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (TimerText)
	{
		float TimeSeconds = GetWorld()->GetTimeSeconds();
		
		// Форматуємо час у mm:ss
		FTimespan Timespan = FTimespan::FromSeconds(TimeSeconds);
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Timespan.GetMinutes(), Timespan.GetSeconds());
		
		TimerText->SetText(FText::FromString(TimeString));
	}
}

void USimPetHUDWidget::UpdateStaminaBar(float NewValue, float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}

void USimPetHUDWidget::UpdateAnimalStats()
{
	if (!GetWorld()) return;
	
	if (DogCountText)
	{
		TArray<AActor *> Dogs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetDog::StaticClass(), Dogs);
		DogCountText->SetText(FText::AsNumber(Dogs.Num()));
	}
	
	if (CanaryCountText)
	{
		TArray<AActor *> Canaries;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetCanary::StaticClass(), Canaries);
		CanaryCountText->SetText(FText::AsNumber(Canaries.Num()));
	}
	
	if (LizardCountText)
	{
		TArray<AActor *> Lizards;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetLizard::StaticClass(), Lizards);
		LizardCountText->SetText(FText::AsNumber(Lizards.Num()));
	}
}
