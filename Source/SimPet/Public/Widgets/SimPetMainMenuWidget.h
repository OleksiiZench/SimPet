// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetMainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;
class UGameUserSettings;

enum class EMainMenuTab : uint8
{
	MainMenu,
	GeneralSettings,
	DifficultySettings,
	GraphSettings
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
	UButton *Btn_NewGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Exit;
	
	// 2. GeneralSettingsMenu
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultySettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphSettings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GeneralSettingsBack;
	
	// 3. DifficultySettingsMenu
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyRight;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Difficulty;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultySettingsBack;
	
	// 4. GraphSettingsMenu
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphRight;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Graph;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_GraphSettingsBack;
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
	
#pragma endregion
	
private:
	UPROPERTY()
	UGameUserSettings *CachedUserSettings;
	
	TArray<FText> DifficultyOptions;
	int32 CurrentDifficultyIndex;
	
	TArray<FText> GraphOptions;
	int32 CurrentGraphIndex;
	
	void CacheUserSettings();
	
	void SwitchToTab(EMainMenuTab TabToSwitch);
	
	void SetupButtonBindings();
	void SetupMainMenuButtonBindings();
	void SetupGeneralSettingsButtonBindings();
	void SetupDifficultySettingsButtonBindings();
	void SetupGraphSettingsButtonBindings();
	
	void InitializeDifficultyOptions();
	void UpdateDifficultyDisplay();
	
	void InitializeGraphOptions();
	void InitializeGraphIndexFromEngine();
	void UpdateGraphDisplay();
	
	void SetGraphSettings(int32 GraphIndex);
	void SetLowGraphSettings();
	void SetMediumGraphSettings();
	void SetHighGraphSettings();
};
