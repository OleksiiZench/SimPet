// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/SimPetBaseCharacter.h"

#include "SimPetPlayer.generated.h"

class UInputComponent;
class UCameraComponent;
class UInputMappingContext;
class USimPetInputConfig;
class USimPetHUDWidget;
class USimPetPauseMenuWidget;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetPlayer : public ASimPetBaseCharacter
{
	GENERATED_BODY()
	
public:
	ASimPetPlayer();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	
	// UPROPERTY(EditDefaultsOnly, Category = "UI")
	// TSubclassOf<USimPetHUDWidget> HUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetPauseMenuWidget> PauseMenuWidgetClass;

private:
	bool bIsGamePaused = false;
	APlayerController *PlayerController;
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *Camera;
	
	// UPROPERTY()
	// USimPetHUDWidget *CurrentHUDWidget;
	
	UPROPERTY()
	USimPetPauseMenuWidget *CurrentPauseMenuWidget;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	USimPetInputConfig *InputConfigDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_Look(const FInputActionValue &InputActionValue);
	void Input_Interact(const FInputActionValue &InputActionValue);
	void Input_Pause(const FInputActionValue &InputActionValue);

#pragma endregion

};
