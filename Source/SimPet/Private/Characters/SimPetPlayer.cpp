// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetInteractable.h"
#include "Controllers/SimPetPlayerController.h"
#include "Components/SimPetInteractionComponent.h"

ASimPetPlayer::ASimPetPlayer()
{
	// 1. Налаштування камери
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;
	
	// 2. Налаштування швидкості
	WalkSpeed = 500.0f;
	SprintSpeed = 800.0f;
	
	// 3. Налаштування коливання камери
	BobVerticalAmplitude = 10.0f;
	BobHorizontalAmplitude = 5.0f;
	BobFrequencyMultiplier = 0.015f;
	BobTimer = 0.0f;
	
	// 4. Налаштування стаміни
	MaxStamina = 3.0f;
	StaminaDrainRate = 1.0f;
	StaminaRegenRate = 1.0f;
	CurrentStamina = MaxStamina;
	
	// 5. Налаштування точки для приєднання items
	ItemHoldPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldPoint"));
	ItemHoldPoint->SetupAttachment(GetCapsuleComponent());
	
	// 6. Налаштування компонента через який відбуваються взаємодії
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
	
	// 4. Записуємо стартову позицію камери (відносно персонажу)
	if (Camera)
		DefaultCameraRelativeLocation = Camera->GetRelativeLocation();
}

void ASimPetPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 1. Оновлюємо коливання камери
	UpdateCameraBob(DeltaTime);
	
	// 2. Оновлення стаміни
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
	if (Camera)
	{
		return Camera->GetComponentLocation();
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

void ASimPetPlayer::UpdateCameraBob(float DeltaTime)
{
	if (!Camera || !CurrentMovementComp)
		return;
	
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size();
	
	bool bIsMovingOnGround = Speed > 10.0f && CurrentMovementComp->IsMovingOnGround();
	
	if (bIsMovingOnGround)
	{
		BobTimer += Speed *DeltaTime * BobFrequencyMultiplier;  // Збільшуємо таймер залежно від швидкості
		
		float ZOffset = FMath::Sin(BobTimer * 2.0f) * BobVerticalAmplitude;  // Вертикальний зсув
		
		float YOffset = FMath::Cos(BobTimer) * BobHorizontalAmplitude;  // Горизонтальний зсув
		
		// Створюємо нову відносну позицію
		FVector NewBobLocation = DefaultCameraRelativeLocation;
		NewBobLocation.Z += ZOffset;
		NewBobLocation.Y += YOffset;
		
		// Застосовуємо нову позицію
		FVector SmoothLocation = FMath::VInterpTo(Camera->GetRelativeLocation(), NewBobLocation, DeltaTime, 10.0f);
		Camera->SetRelativeLocation(SmoothLocation);
	}
	else
	{// Коли зупинились
		FVector SmoothReturn = FMath::VInterpTo(Camera->GetRelativeLocation(), DefaultCameraRelativeLocation, DeltaTime, 5.0f);
	
		if (FVector::DistSquared(SmoothReturn, DefaultCameraRelativeLocation) < 1.0f)
		{
			Camera->SetRelativeLocation(DefaultCameraRelativeLocation);
		}
		else
		{
			Camera->SetRelativeLocation(SmoothReturn);
		}
	}
}
