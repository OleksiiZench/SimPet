// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"

ASimPetPlayer::ASimPetPlayer()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;
}

void ASimPetPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Adding Mapping Context
	if (APlayerController *PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	// Configure EnhancedInputComponent
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (!InputConfigDataAsset)
			return;

		const UInputAction *MoveAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Move);
		if (MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASimPetPlayer::Input_Move);

		const UInputAction *LookAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Look);
		if (LookAction)
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASimPetPlayer::Input_Look);
	}
}

void ASimPetPlayer::Input_Move(const FInputActionValue &InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ASimPetPlayer::Input_Look(const FInputActionValue &InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
