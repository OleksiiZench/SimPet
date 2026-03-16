// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetInteractable.h"
#include "Widgets/SimPetPauseMenuWidget.h"
#include "Controllers/SimPetPlayerController.h"
#include "Items/SimPetItem.h"

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

void ASimPetPlayer::InteractWithItem(ASimPetItem *Item)
{
	if (TryItemInteraction(Item))
	{
		return;
	}
	
	TakeOrDropItem(Item);
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

bool ASimPetPlayer::TryItemInteraction(ASimPetItem *TargetItem)
{
	if (!bHandsFull || CashedTakenItem == nullptr || CashedTakenItem == TargetItem)
	{
		return false;
	}
	
	return CashedTakenItem->TryInteractWithAnotherItem(TargetItem);
}

void ASimPetPlayer::TakeOrDropItem(ASimPetItem *Item)
{
	if (bHandsFull && CashedTakenItem == Item)
	{
		DropItem(Item);
	}
	else if (!bHandsFull)
	{
		TakeItem(Item);
	}
	else
	{
		DropItem(CashedTakenItem);
		TakeItem(Item);
	}
}

void ASimPetPlayer::TakeItem(ASimPetItem *Item)
{
	Item->DisablePhysics();
	Item->AttachToComponent(ItemHoldPoint, GetRuleForAttachingItems());
	
	CashedTakenItem = Item;
	bHandsFull = true;
}

void ASimPetPlayer::DropItem(ASimPetItem *Item)
{
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->EnablePhysics();
	
	CashedTakenItem = nullptr;
	bHandsFull = false;
}

FAttachmentTransformRules ASimPetPlayer::GetRuleForAttachingItems()
{
	return FAttachmentTransformRules (
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		true
	);
}

AActor *ASimPetPlayer::DoInteractionTrace(const FInputActionValue &InputActionValue)
{
	if (!InputActionValue.Get<bool>())
		return nullptr;

	float InteractionDistance = 250.0f;
	float InteractionRadius = 15.0f;
	FHitResult HitResult;
	FVector StartLocation = Camera->GetComponentLocation();
	FVector EndLocation = StartLocation + Camera->GetForwardVector() * InteractionDistance;

	TArray<AActor *> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		InteractionRadius,
		ETraceTypeQuery::TraceTypeQuery2,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (bHit)
	{
		AActor *HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->Implements<USimPetInteractable>())
			return HitActor;
	}
	
	return nullptr;
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
	AActor *HitActor = DoInteractionTrace(InputActionValue);

	if (HitActor)
	{
		ISimPetInteractable::Execute_Interact(HitActor, this);
	}
}

void ASimPetPlayer::Input_SecondaryInteract(const FInputActionValue &InputActionValue)
{
	AActor *HitActor = DoInteractionTrace(InputActionValue);

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
