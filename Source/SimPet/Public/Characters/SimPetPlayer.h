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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;
	
	// UPROPERTY(EditDefaultsOnly, Category = "UI")
	// TSubclassOf<USimPetHUDWidget> HUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetPauseMenuWidget> PauseMenuWidgetClass;
	
	// Налаштування швидкості персонажа
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;

private:
	bool bIsGamePaused = false;
	//bool bIsSprinting = false;
	ASimPetPlayerController *SimPetPC;
	UCharacterMovementComponent *CurrentMovementComp;
	
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

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	// UInputMappingContext *DefaultMappingContext;

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
