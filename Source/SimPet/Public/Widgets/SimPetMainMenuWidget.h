// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetMainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UGameUserSettings;
class USimPetSaveSubsystem;

enum class EMainMenuTab : uint8
{
	MainMenu,
	GeneralSettings,
	DifficultySettings,
	GraphSettings,
	LanguageSettings,
};

enum class EGraphicsQuality : uint8
{
	Low,
	Medium,
	High
};




UCLASS()
class SIMPET_API USimPetMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FName GameplayLevelName;
	
#pragma region UI Bindings
	// 1. MainMenu
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher *MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Continue;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_Continue;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_NewGame;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_NewGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Exit;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_Exit;
	
	// 2. GeneralSettingsMenu
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_GeneralSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultySettings;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_DifficultySettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphSettings;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_GraphSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_LanguageSettings;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_LanguageSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GeneralSettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_GeneralSettingsBack;
	
	// 3. DifficultySettingsMenu
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_DifficultySettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyRight;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Difficulty;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultySettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_DifficultySettingsBack;
	
	// 4. GraphSettingsMenu
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_GraphSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphRight;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Graph;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphSettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_GraphSettingsBack;
	
	// 5. LanguageSettingsMenu
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_LanguageSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_LanguageLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_LanguageRight;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Language;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_LanguageSettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_LanguageSettingsBack;
#pragma endregion
	
#pragma region UI Callbacks
	// 1. MainMenu
	UFUNCTION()
	void OnContinueClicked();
	
	UFUNCTION()
	void OnNewGameClicked();
	
	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnExitClicked();
	
	// 2. GeneralSettingsMenu
	UFUNCTION()
	void OnDifficultySettingsClicked();
	
	UFUNCTION()
	void OnGraphSettingsClicked();
	
	UFUNCTION()
	void OnLanguageSettingsClicked();
	
	UFUNCTION()
	void OnGeneralSettingsBackClicked();
	
	// 3. DifficultySettingsMenu
	UFUNCTION()
	void OnDifficultyLeftClicked();
	
	UFUNCTION()
	void OnDifficultyRightClicked();
	
	UFUNCTION()
	void OnDifficultyBackClicked();
	
	// 4. GraphSettingsMenu
	UFUNCTION()
	void OnGraphLeftClicked();
	
	UFUNCTION()
	void OnGraphRightClicked();
	
	UFUNCTION()
	void OnGraphBackClicked();
	
	// 5. LanguageSettingsMenu
	UFUNCTION()
	void OnLanguageLeftClicked();
	
	UFUNCTION()
	void OnLanguageRightClicked();
	
	UFUNCTION()
	void OnLanguageBackClicked();
	
#pragma endregion
	
private:
	UPROPERTY()
	UGameUserSettings *CachedUserSettings;
	
	UPROPERTY()
	USimPetSaveSubsystem *CachedSaveSubsystem;
	
	TArray<FText> DifficultyOptions;
	int32 CurrentDifficultyIndex;
	
	TArray<FText> GraphOptions;
	int32 CurrentGraphIndex;
	
	TArray<FText> LanguageOptions;
	int32 CurrentLanguageIndex;
	
	void CacheUserSettings();
	void CacheSaveSubsystem();
	
	void SwitchToTab(EMainMenuTab TabToSwitch);
	
	void SetupBindings();
	void SetupMainMenuBindings();
	void SetupGeneralSettingsBindings();
	void SetupDifficultySettingsBindings();
	void SetupGraphSettingsBindings();
	void SetupLanguageSettingsBindings();
	
	void InitializeContinueButton();
	
	void InitializeDifficultyOptions();
	void InitializeDifficultyIndexFromSaveSubsystem();
	void UpdateDifficultyDisplay();
	
	void InitializeGraphOptions();
	void InitializeGraphIndexFromEngine();
	void UpdateGraphDisplay();
	
	void InitializeLanguageOptions();
	void InitializeLanguageIndexFromSaveInternationalization();
	void UpdateLanguageDisplay();
	
	void SetGraphSettings();
	void SetLowGraphSettings();
	void SetMediumGraphSettings();
	void SetHighGraphSettings();
	void SaveDifficultySettings();
	void SetLanguageSettings();
};
