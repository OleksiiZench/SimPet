// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/SimPetBaseCharacter.h"

#include "SimPetPlayer.generated.h"

class UInputComponent;
class USimPetInputConfig;
class ASimPetPlayerController;
class USceneComponent;
class ASimPetItem;
class USimPetInteractionComponent;
class USimPetCameraComponent;
class USimPetStaminaComponent;

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
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
	
	USimPetStaminaComponent *GetStaminaComponent();
	
private:	
#pragma region Components
	UCharacterMovementComponent *CurrentMovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent *ItemHoldPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USimPetCameraComponent *CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USimPetInteractionComponent *InteractionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USimPetStaminaComponent *StaminaComponent;
	
	void SetupComponents();
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	USimPetInputConfig *InputConfigDataAsset;

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_Look(const FInputActionValue &InputActionValue);
	void Input_Interact(const FInputActionValue &InputActionValue);
	void Input_SecondaryInteract(const FInputActionValue &InputActionValue);
	void Input_Sprint_Started(const FInputActionValue &InputActionValue);
	void Input_Sprint_Completed(const FInputActionValue &InputActionValue);

#pragma endregion
};
