// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetPauseMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/SimPetPlayer.h"

void USimPetPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 1. Прив'язка кнопок
	if (Btn_Resume)
		Btn_Resume->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnResumeClicked);
	
	if (Btn_ToMainMenu)
		Btn_ToMainMenu->OnClicked.AddDynamic(this, &USimPetPauseMenuWidget::OnToMainMenuClicked);
}

void USimPetPauseMenuWidget::OnResumeClicked()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (ASimPetPlayer *SimPetPlayerCharacter = Cast<ASimPetPlayer>(PlayerCharacter))
	{
		SimPetPlayerCharacter->TogglePauseMenu();
	}
}

void USimPetPauseMenuWidget::OnToMainMenuClicked()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
