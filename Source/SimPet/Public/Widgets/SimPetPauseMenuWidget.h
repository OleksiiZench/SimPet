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
	
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Resume;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_SaveGame;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_ToMainMenu;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *MaxPointsText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *CurrentPointsText;
#pragma endregion
	
#pragma region UI Callbacks
	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnToMainMenuClicked();
	
	UFUNCTION()
	void OnSaveGameClicked();
#pragma endregion
	
private:
	UPROPERTY()
	ASimPetPlayerController *CachedPlayerController;
	
	UPROPERTY()
	USimPetSaveSubsystem *CachedSaveSubsystem;
	
	UPROPERTY()
	ASimPetPlayerState *CachedPlayerState;
	
	void SetupButtonBindings();
	
	void UpdateAssignedText();
	void UpdateMaxPointsText();
	void UpdateCurrentPointsText();
	
	void CachePlayerController();
	void CacheSaveSubsystem();
	void CachePlayerState();
};
