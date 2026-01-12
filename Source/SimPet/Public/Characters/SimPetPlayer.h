// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/SimPetBaseCharacter.h"

#include "SimPetPlayer.generated.h"

class UCameraComponent;
class UInputMappingContext;
class USimPetInputConfig;
class USimPetHUDWidget;

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

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetHUDWidget> HUDWidgetClass;

private:

	APlayerController *PlayerController;
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *Camera;
	
	UPROPERTY()
	USimPetHUDWidget *CurrentHUD;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	USimPetInputConfig *InputConfigDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_Look(const FInputActionValue &InputActionValue);
	void Input_Interact(const FInputActionValue &InputActionValue);

#pragma endregion

};
