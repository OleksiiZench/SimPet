// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetMainMenuWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"

#include "SimPetDebugHelper.h"

void USimPetMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetupButtonBindings();
	
	InitializeDifficultyOptions();
	InitializeGraphOptions();
}

void USimPetMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Btn_Continue)
		Btn_Continue->SetUserFocus(GetOwningPlayer());
}

void USimPetMainMenuWidget::OnContinueClicked()
{
	Debug::Print(TEXT("Continue Button Clicked! (Save system not implemented yet)"));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

void USimPetMainMenuWidget::OnSettingsClicked()
{
	SwitchToTab(EMainMenuTab::GeneralSettings);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetMainMenuWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USimPetMainMenuWidget::OnDifficultySettingsClicked()
{
	SwitchToTab(EMainMenuTab::DifficultySettings);
}

void USimPetMainMenuWidget::OnGraphSettingsClicked()
{
	SwitchToTab(EMainMenuTab::GraphSettings);
}

void USimPetMainMenuWidget::OnGeneralSettingsBackClicked()
{
	SwitchToTab(EMainMenuTab::MainMenu);
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

void USimPetMainMenuWidget::OnDifficultyBackClicked()
{
	SwitchToTab(EMainMenuTab::GeneralSettings);
}

void USimPetMainMenuWidget::OnGraphLeftClicked()
{
	CurrentGraphIndex--;
	if (CurrentGraphIndex < 0)
		CurrentGraphIndex = GraphOptions.Num() - 1;
	
	UpdateGraphDisplay();
}

void USimPetMainMenuWidget::OnGraphRightClicked()
{
	CurrentGraphIndex++;
	if (CurrentGraphIndex >= GraphOptions.Num())
		CurrentGraphIndex = 0;
	
	UpdateGraphDisplay();
}

void USimPetMainMenuWidget::OnGraphBackClicked()
{
	SetGraphSettings(CurrentGraphIndex);
	
	SwitchToTab(EMainMenuTab::GeneralSettings);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetMainMenuWidget::SwitchToTab(EMainMenuTab TabToSwitch)
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidgetIndex(static_cast<int32>(TabToSwitch));
	}
}

void USimPetMainMenuWidget::SetupButtonBindings()
{
	SetupMainMenuButtonBindings();
	SetupGeneralSettingsButtonBindings();
	SetupDifficultySettingsButtonBindings();
	SetupGraphSettingsButtonBindings();
}

void USimPetMainMenuWidget::SetupMainMenuButtonBindings()
{
	if (Btn_Continue)
		Btn_Continue->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnContinueClicked);
	
	if (Btn_NewGame)
		Btn_NewGame->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnNewGameClicked);
	
	if (Btn_Settings)
		Btn_Settings->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnSettingsClicked);
	
	if (Btn_Exit)
		Btn_Exit->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnExitClicked);
}

void USimPetMainMenuWidget::SetupGeneralSettingsButtonBindings()
{
	if (Btn_DifficultySettings)
		Btn_DifficultySettings->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultySettingsClicked);
	
	if (Btn_GraphSettings)
		Btn_GraphSettings->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnGraphSettingsClicked);
	
	if (Btn_GeneralSettingsBack)
		Btn_GeneralSettingsBack->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnGeneralSettingsBackClicked);
}

void USimPetMainMenuWidget::SetupDifficultySettingsButtonBindings()
{
	if (Btn_DifficultyLeft)
		Btn_DifficultyLeft->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultyLeftClicked);
	
	if (Btn_DifficultyRight)
		Btn_DifficultyRight->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultyRightClicked);
	
	if (Btn_DifficultySettingsBack)
		Btn_DifficultySettingsBack->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnDifficultyBackClicked);
}

void USimPetMainMenuWidget::SetupGraphSettingsButtonBindings()
{
	if (Btn_GraphLeft)
		Btn_GraphLeft->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnGraphLeftClicked);
	
	if (Btn_GraphRight)
		Btn_GraphRight->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnGraphRightClicked);
	
	if (Btn_GraphSettingsBack)
		Btn_GraphSettingsBack->OnClicked.AddDynamic(this, &USimPetMainMenuWidget::OnGraphBackClicked);
}

void USimPetMainMenuWidget::InitializeDifficultyOptions()
{
	DifficultyOptions.Empty();
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Easy", "Easy"));
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Normal", "Normal"));
	DifficultyOptions.Add(NSLOCTEXT("Settings", "Diff_Hard", "Hard"));
	
	UpdateDifficultyDisplay();
}

void USimPetMainMenuWidget::UpdateDifficultyDisplay()
{
	if (Text_Difficulty && DifficultyOptions.IsValidIndex(CurrentDifficultyIndex))
	{
		Text_Difficulty->SetText(DifficultyOptions[CurrentDifficultyIndex]);
	}
}

void USimPetMainMenuWidget::InitializeGraphOptions()
{
	GraphOptions.Empty();
	GraphOptions.Add(NSLOCTEXT("Settings", "Graph_Low", "Low"));
	GraphOptions.Add(NSLOCTEXT("Settings", "Graph_Medium", "Medium"));
	GraphOptions.Add(NSLOCTEXT("Settings", "Graph_High", "High"));
	
	UpdateGraphDisplay();
}

void USimPetMainMenuWidget::UpdateGraphDisplay()
{
	if (Text_Graph && GraphOptions.IsValidIndex(CurrentGraphIndex))
	{
		Text_Graph->SetText(GraphOptions[CurrentGraphIndex]);
	}
}

void USimPetMainMenuWidget::SetGraphSettings(int32 GraphIndex)
{
	EGraphicsQuality QualityLevel = static_cast<EGraphicsQuality>(GraphIndex);
	switch (QualityLevel)
	{
	case EGraphicsQuality::Low:
		SetLowGraphSettings();
		break;
		
	case EGraphicsQuality::Medium:
		SetMediumGraphSettings();
		break;
		
	case EGraphicsQuality::High:
		SetHighGraphSettings();
		break;
		
	default:
		Debug::PrintError(TEXT("Invalid graph index"));
		break;
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void USimPetMainMenuWidget::SetLowGraphSettings()
{
	UGameUserSettings *UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetViewDistanceQuality(1);
		UserSettings->SetAntiAliasingQuality(1);
		UserSettings->SetPostProcessingQuality(0);
		UserSettings->SetShadowQuality(0);
		UserSettings->SetGlobalIlluminationQuality(0);
		UserSettings->SetReflectionQuality(1);
		UserSettings->SetTextureQuality(1);
		UserSettings->SetVisualEffectQuality(1);
		UserSettings->SetFoliageQuality(1);
		UserSettings->SetShadingQuality(1);

		UserSettings->ApplyNonResolutionSettings();
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void USimPetMainMenuWidget::SetMediumGraphSettings()
{
	UGameUserSettings *UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetViewDistanceQuality(2);
		UserSettings->SetAntiAliasingQuality(2);
		UserSettings->SetPostProcessingQuality(1);
		UserSettings->SetShadowQuality(1);
		UserSettings->SetGlobalIlluminationQuality(1);
		UserSettings->SetReflectionQuality(2);
		UserSettings->SetTextureQuality(2);
		UserSettings->SetVisualEffectQuality(2);
		UserSettings->SetFoliageQuality(2);
		UserSettings->SetShadingQuality(2);

		UserSettings->ApplyNonResolutionSettings();
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void USimPetMainMenuWidget::SetHighGraphSettings()
{
	UGameUserSettings *UserSettings = GEngine->GetGameUserSettings();
	if (UserSettings)
	{
		UserSettings->SetViewDistanceQuality(3);
		UserSettings->SetAntiAliasingQuality(3);
		UserSettings->SetPostProcessingQuality(3);
		UserSettings->SetShadowQuality(3);
		UserSettings->SetGlobalIlluminationQuality(3);
		UserSettings->SetReflectionQuality(3);
		UserSettings->SetTextureQuality(3);
		UserSettings->SetVisualEffectQuality(3);
		UserSettings->SetFoliageQuality(3);
		UserSettings->SetShadingQuality(3);

		UserSettings->ApplyNonResolutionSettings();
	}
}
