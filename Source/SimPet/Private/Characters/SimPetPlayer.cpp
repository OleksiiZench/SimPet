// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetInteractable.h"
#include "Controllers/SimPetPlayerController.h"
#include "Components/SimPetInteractionComponent.h"
#include "Components/SimPetCameraComponent.h"

ASimPetPlayer::ASimPetPlayer()
{
	// 1. Налаштування камери
	CameraComponent = CreateDefaultSubobject<USimPetCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->bUsePawnControlRotation = true;
	
	// 2. Налаштування швидкості
	WalkSpeed = 500.0f;
	SprintSpeed = 800.0f;
	
	// 3. Налаштування стаміни
	MaxStamina = 3.0f;
	StaminaDrainRate = 1.0f;
	StaminaRegenRate = 1.0f;
	CurrentStamina = MaxStamina;
	
	// 4. Налаштування точки для приєднання items
	ItemHoldPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldPoint"));
	ItemHoldPoint->SetupAttachment(GetCapsuleComponent());
	
	// 5. Налаштування компонента через який відбуваються взаємодії
	InteractionComponent = CreateDefaultSubobject<USimPetInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetHoldPoint(ItemHoldPoint);
}

void ASimPetPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. Ініціалізуємо PC
	SimPetPC = Cast<ASimPetPlayerController>(GetController());
	
	// 2. Отримуємо MovementComponent
	CurrentMovementComp = GetCharacterMovement();
	
	// 3. Встановлюємо стартову швидкість
	CurrentMovementComp->MaxWalkSpeed = WalkSpeed;
}

void ASimPetPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateStamina(DeltaTime);
}

void ASimPetPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{// Configure EnhancedInputComponent
		if (!InputConfigDataAsset)
			return;

		const UInputAction *MoveAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Move);
		if (MoveAction)
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASimPetPlayer::Input_Move);

		const UInputAction *LookAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Look);
		if (LookAction)
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASimPetPlayer::Input_Look);

		const UInputAction *InteractAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Interact);
		if (InteractAction)
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASimPetPlayer::Input_Interact);
		
		const UInputAction *SecondaryInteractAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_SecondaryInteract);
		if (SecondaryInteractAction)
			EnhancedInputComponent->BindAction(SecondaryInteractAction, ETriggerEvent::Started, this, &ASimPetPlayer::Input_SecondaryInteract);
		
		const UInputAction *SprintAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Sprint);
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASimPetPlayer::Input_Sprint_Started);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASimPetPlayer::Input_Sprint_Completed);
		}
	}
}

FVector ASimPetPlayer::GetPawnViewLocation() const
{
	if (CameraComponent)
	{
		return CameraComponent->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation();
}

void ASimPetPlayer::InteractWithItem(ASimPetItem *Item)
{
	if (InteractionComponent->TryItemInteraction(Item))
	{
		return;
	}
	
	InteractionComponent->TakeOrDropItem(Item);
}

void ASimPetPlayer::SetSprint(bool bIsSprint)
{
	bool bIsMoving = !(GetVelocity().Size2D() < 5.0f);
	
	if (bIsSprint && bIsMoving)
	{
		CurrentMovementComp->MaxWalkSpeed = SprintSpeed;
		bIsSprinting = true;
	}
	else
	{
		CurrentMovementComp->MaxWalkSpeed = WalkSpeed;
		bIsSprinting = false;
	}
}

void ASimPetPlayer::UpdateStamina(float DeltaTime)
{
	float OldStamina = CurrentStamina;
	
	if (bIsSprinting)
	{
		bool bIsMoving = GetVelocity().Size2D();
		bool bIsFalling = CurrentMovementComp->IsFalling();
		
		if (!bIsMoving || bIsFalling)
		{
			SetSprint(false);
		}
		else
		{
			CurrentStamina -= StaminaDrainRate * DeltaTime;
		
			if (CurrentStamina <= 0.0f)
			{
				CurrentStamina = 0.0f;
				SetSprint(false);
			}
		}
	}
	else if (CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * DeltaTime;
	}
	
	if (!FMath::IsNearlyEqual(OldStamina, CurrentStamina))
	{
		float StaminaPercent = FMath::Clamp(CurrentStamina / MaxStamina, 0.0f, 1.0f);
		
		if (OnStaminaChanged.IsBound())
			OnStaminaChanged.Broadcast(CurrentStamina, StaminaPercent);
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

void ASimPetPlayer::Input_Interact(const FInputActionValue &InputActionValue)
{
	AActor *HitActor = InteractionComponent->DoInteractionTrace();

	if (HitActor)
	{
		ISimPetInteractable::Execute_Interact(HitActor, this);
	}
}

void ASimPetPlayer::Input_SecondaryInteract(const FInputActionValue &InputActionValue)
{
	AActor *HitActor = InteractionComponent->DoInteractionTrace();

	if (HitActor)
	{
		ISimPetInteractable::Execute_SecondaryInteract(HitActor, this);
	}
}

void ASimPetPlayer::Input_Sprint_Started(const FInputActionValue& InputActionValue)
{
	SetSprint(true);
}

void ASimPetPlayer::Input_Sprint_Completed(const FInputActionValue& InputActionValue)
{
	SetSprint(false);
}
