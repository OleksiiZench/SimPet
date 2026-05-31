// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SceneComponent.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetInteractable.h"
#include "Components/SimPetInteractionComponent.h"
#include "Components/SimPetCameraComponent.h"
#include "Components/Attributes/SimPetStaminaComponent.h"
#include "Save/SimPetSaveGame.h"
#include "Subsystems/SimPetSaveSubsystem.h"
#include "Save/Structures/SimPetPlayerSaveData.h"

#include "Items/SimPetItem.h"

ASimPetPlayer::ASimPetPlayer()
{
	SetupComponents();
}

void ASimPetPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentMovementComp = GetCharacterMovement();
	
	CacheSaveSubsystem();
	
	RegisterWithSaveSystem();
}

void ASimPetPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterFromSaveSystem();
	
	Super::EndPlay(EndPlayReason);
}

void ASimPetPlayer::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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

void ASimPetPlayer::SaveActorData_Implementation(USimPetSaveGame *SaveObject)
{
	if (SaveObject == nullptr)
		return;
	
	FSimPetPlayerSaveData PlayerSaveData;
	
	PlayerSaveData.Transform = GetActorTransform();
	PlayerSaveData.EquippedItemData.ItemClass = InteractionComponent->GetTakenItemClass();
	// TODO: Потрібно заповнити PlayerSaveData.EquippedItemData.PayloadClasses
	PlayerSaveData.Stamina = StaminaComponent->GetCurrentStamina();
	PlayerSaveData.ControlRotation = GetControlRotation();
	
	SaveObject->PlayerSaveData = PlayerSaveData;
	
}

void ASimPetPlayer::LoadActorData_Implementation(USimPetSaveGame *SaveObject)
{
	if (SaveObject == nullptr)
		return;
	
	FSimPetPlayerSaveData PlayerSaveData = SaveObject->PlayerSaveData;
	
	SetActorLocation(PlayerSaveData.Transform.GetLocation());
	
	TSubclassOf<ASimPetItem> ItemClass = PlayerSaveData.EquippedItemData.ItemClass;
	ASimPetItem *SpawnedItem = GetWorld()->SpawnActor<ASimPetItem>(*ItemClass, GetTransform());
	InteractionComponent->TakeOrDropOrSwapItem(SpawnedItem);
	
	StaminaComponent->SetCurrentStamina(PlayerSaveData.Stamina);
	
	FaceRotation(PlayerSaveData.ControlRotation);
}

USimPetStaminaComponent *ASimPetPlayer::GetStaminaComponent() const
{
	return StaminaComponent;
}

void ASimPetPlayer::SetupComponents()
{
	CameraComponent = CreateDefaultSubobject<USimPetCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->bUsePawnControlRotation = true;
	
	ItemHoldPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemHoldPoint"));
	ItemHoldPoint->SetupAttachment(GetCapsuleComponent());
	
	InteractionComponent = CreateDefaultSubobject<USimPetInteractionComponent>(TEXT("InteractionComponent"));
	InteractionComponent->SetHoldPoint(ItemHoldPoint);
	
	StaminaComponent = CreateDefaultSubobject<USimPetStaminaComponent>(TEXT("StaminaComponent"));
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
		if (InteractionComponent->TryUseEquippedItemOn(HitActor))
			return;
		
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

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetPlayer::Input_Sprint_Started(const FInputActionValue& InputActionValue)
{
	StaminaComponent->SetSprint(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetPlayer::Input_Sprint_Completed(const FInputActionValue& InputActionValue)
{
	StaminaComponent->SetSprint(false);
}

void ASimPetPlayer::CacheSaveSubsystem()
{
	if (GetWorld())
	{
		CachedSaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	}
}

void ASimPetPlayer::RegisterWithSaveSystem()
{
	if (CachedSaveSubsystem)
	{
		CachedSaveSubsystem->RegisterSavableActor(this);
	}
}

void ASimPetPlayer::UnregisterFromSaveSystem()
{
	if (CachedSaveSubsystem)
	{
		CachedSaveSubsystem->UnregisterSavableActor(this);
	}
}
