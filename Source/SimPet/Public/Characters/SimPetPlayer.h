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
class ASimPetPlayerController;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentValue, float, Precent);

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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	
#pragma region Delegates
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaChanged OnStaminaChanged;
#pragma endregion

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetPauseMenuWidget> PauseMenuWidgetClass;
	
	/* Налаштування швидкості персонажа */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;
	
	/* Налаштування стаміни персонажа */
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxStamina;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaDrainRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaRegenRate;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float CurrentStamina;
private:
	bool bIsGamePaused = false;
	bool bIsSprinting = false;
	ASimPetPlayerController *SimPetPC;
	UCharacterMovementComponent *CurrentMovementComp;
	
	void SetSprint(bool bIsSprint);
	void UpdateStamina(float DeltaTime);
	
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *Camera;
	
	UPROPERTY()
	USimPetPauseMenuWidget *CurrentPauseMenuWidget;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	USimPetInputConfig *InputConfigDataAsset;

	void Input_Move(const FInputActionValue &InputActionValue);
	void Input_Look(const FInputActionValue &InputActionValue);
	void Input_Interact(const FInputActionValue &InputActionValue);
	void Input_Pause(const FInputActionValue &InputActionValue);
	void Input_Sprint_Started(const FInputActionValue &InputActionValue);
	void Input_Sprint_Completed(const FInputActionValue &InputActionValue);

#pragma endregion
	
#pragma region Bobbing
	float BobVerticalAmplitude;  // Наскільки сильно коливає вгору/ вниз
	float BobHorizontalAmplitude;  // Наскільки сильно коливає вліво/ вправо
	float BobFrequencyMultiplier;  // Множник швидкості коливання
	float BobTimer;  // Таймер для синумоїди
	FVector DefaultCameraRelativeLocation;  // Координати камери в спокої
	
	void UpdateCameraBob(float DeltaTime);
#pragma endregion
};
