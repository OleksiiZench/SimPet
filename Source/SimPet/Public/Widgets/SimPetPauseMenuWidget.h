// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetPauseMenuWidget.generated.h"

class UButton;
class UTextBlock;
class ASimPetPlayerController;
class USimPetSaveSubsystem;
class ASimPetPlayerState;
class USimPetHelpWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FName MainMenuLevelName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<USimPetHelpWidget> HelpWidgetClass;
	
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Pause;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Resume;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_Resume;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_SaveGame;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_SaveGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_ToMainMenu;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_ToMainMenu;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Help;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TextBtn_Help;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_MaxPoints;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_CountMaxPoints;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_CurrentPoints;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_CountCurrentPoints;
#pragma endregion
	
#pragma region UI Callbacks
	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnToMainMenuClicked();
	
	UFUNCTION()
	void OnSaveGameClicked();
	
	UFUNCTION()
	void OnHelpClicked();
#pragma endregion
	
private:
	UPROPERTY()
	ASimPetPlayerController *CachedPlayerController;
	
	UPROPERTY()
	USimPetSaveSubsystem *CachedSaveSubsystem;
	
	UPROPERTY()
	ASimPetPlayerState *CachedPlayerState;
	
	UPROPERTY()
	USimPetHelpWidget* HelpWidgetInstance;
	
	UFUNCTION()
	void OnHelpMenuClosedFromPause();
	
	void SetupButtonBindings();
	void SetupUIBindings();
	
	void UpdateAssignedText();
	void UpdateMaxPointsText();
	void UpdateCurrentPointsText();
	
	void CachePlayerController();
	void CacheSaveSubsystem();
	void CachePlayerState();
};
