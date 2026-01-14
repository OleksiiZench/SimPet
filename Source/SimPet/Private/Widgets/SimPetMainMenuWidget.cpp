// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "SimPetDebugHelper.h"
#include "Components/WidgetSwitcher.h"

void USimPetMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 1. Прив'язка кнопок
	if (Btn_Continue)
		Btn_Continue->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnContinueClicked);
	
	if (Btn_NewGame)
		Btn_NewGame->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnNewGameClicked);
	
	if (Btn_Settings)
		Btn_Settings->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnSettingsClicked);
	
	if (Btn_SettingsBack)
		Btn_SettingsBack->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnSettingsBackClicked);
	
	if (Btn_Exit)
		Btn_Exit->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnExitClicked);
	
	if (Btn_DifficultyLeft)
		Btn_DifficultyLeft->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultyLeftClicked);
	
	if (Btn_DifficultyRight)
		Btn_DifficultyRight->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultyRightClicked);
	
	// 2. Ініціалізація списку складностей
	DifficultyOptions.Empty();
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Easy", "Easy"));
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Normal", "Normal"));
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Hard", "Hard"));
	
	UpdateDifficultyDisplay();
	
	if (Btn_Continue)
		Btn_Continue->SetUserFocus(GetOwningPlayer());
}

void USimPetMainMenuWidget::OnContinueClicked()
{
	Debug::Print(TEXT("Continue Button Clicked! (Save system not implemented yet)"));
}

void USimPetMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

void USimPetMainMenuWidget::OnSettingsClicked()
{
	if (MenuSwitcher)
		MenuSwitcher->SetActiveWidgetIndex(1);
}

void USimPetMainMenuWidget::OnSettingsBackClicked()
{
	if (MenuSwitcher)
		MenuSwitcher->SetActiveWidgetIndex(0);
}

void USimPetMainMenuWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USimPetMainMenuWidget::OnDifficultyLeftClicked()
{
	CurrentDifficultyIndex--;
	if (CurrentDifficultyIndex < 0)
		CurrentDifficultyIndex = DifficultyOptions.Num() - 1;
	
	UpdateDifficultyDisplay();
}

void USimPetMainMenuWidget::OnDifficultyRightClicked()
{
	CurrentDifficultyIndex++;
	if (CurrentDifficultyIndex >= DifficultyOptions.Num())
		CurrentDifficultyIndex = 0;
	
	UpdateDifficultyDisplay();
}

void USimPetMainMenuWidget::UpdateDifficultyDisplay()
{
	if (Text_Difficulty && DifficultyOptions.IsValidIndex(CurrentDifficultyIndex))
	{
		Text_Difficulty->SetText(DifficultyOptions[CurrentDifficultyIndex]);
	}
}
