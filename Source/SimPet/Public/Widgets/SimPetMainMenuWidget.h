// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetMainMenuWidget.generated.h"

class UButton;
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
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher *MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Continue;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_NewGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Settings;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_SettingsBack;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Exit;
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
#pragma endregion
};
