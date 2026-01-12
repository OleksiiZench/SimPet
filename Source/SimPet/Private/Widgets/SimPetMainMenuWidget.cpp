// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "SimPetDebugHelper.h"
#include "Components/WidgetSwitcher.h"

void USimPetMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
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
