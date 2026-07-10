// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SimPetPlayerController.generated.h"

class UInputMappingContext;
class USimPetInputConfig;
class UUserWidget;
class USimPetPauseMenuWidget;
class USimPetHelpWidget;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	void SetInputMode_Game();
	void SetInputMode_UI(UUserWidget *FocusWidget = nullptr);
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	
	UFUNCTION()
	void OnStartupHelpClosed();
	
private:
#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	USimPetInputConfig *InputConfigDataAsset;
	
	void Input_Pause(const FInputActionValue &InputActionValue);
	void Input_SaveGame(const FInputActionValue &InputActionValue);
	
#pragma endregion
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetPauseMenuWidget> PauseMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetHelpWidget> HelpWidgetClass;
	
	UPROPERTY()
	USimPetPauseMenuWidget *CurrentPauseMenuWidget;
	
	void ShowStartupHelp();
};
