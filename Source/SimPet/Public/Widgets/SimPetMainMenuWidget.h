// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetMainMenuWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
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
	
	// 2. SettingsMenu
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyLeft;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_DifficultyRight;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_SettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Difficulty;
#pragma endregion
	
#pragma region UI Callbacks
	UFUNCTION()
	void OnContinueClicked();
	
	UFUNCTION()
	void OnNewGameClicked();
	
	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnSettingsBackClicked();
	
	UFUNCTION()
	void OnExitClicked();
	
	UFUNCTION()
	void OnDifficultyLeftClicked();
	
	UFUNCTION()
	void OnDifficultyRightClicked();
#pragma endregion
	
private:
	TArray<FText> DifficultyOptions;
	int32 CurrentDifficultyIndex;
	
	void SetupButtonBindings();
	void InitializeDifficultyOptions();
	void UpdateDifficultyDisplay();
};
