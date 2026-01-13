// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/SimPetPlayer.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"

#include "DataAsset/Input/SimPetInputConfig.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/SimPetHUDWidget.h"
#include "Widgets/SimPetPauseMenuWidget.h"

#include "SimPetDebugHelper.h"

ASimPetPlayer::ASimPetPlayer()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;
}

void ASimPetPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. Ініціалізуємо PC
	PlayerController = Cast<APlayerController>(GetController());
	
	// 2. Виводимо HUD
	if (IsLocallyControlled() && HUDWidgetClass)
	{
		if (PlayerController)
		{
			CurrentHUDWidget = CreateWidget<USimPetHUDWidget>(PlayerController, HUDWidgetClass);
			if (CurrentHUDWidget)
				CurrentHUDWidget->AddToViewport();
		}
	}
	
	// 3. Налаштування інпуту при старті
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		
		FInputModeGameOnly GameModeInput;
		PlayerController->SetInputMode(GameModeInput);
	}
}

void ASimPetPlayer::TogglePauseMenu()
{
	if (!PlayerController)
		return;
	
	bIsGamePaused = !bIsGamePaused;
	
	// 1. Ставимо/ Знімаємо паузу гри
	UGameplayStatics::SetGamePaused(GetWorld(), bIsGamePaused);
	
	// 2. Логіка відображення
	if (bIsGamePaused)
	{
		if (CurrentHUDWidget)
			CurrentHUDWidget->SetVisibility(ESlateVisibility::Hidden);
		
		if (!CurrentPauseMenuWidget && PauseMenuWidgetClass)
		{
			CurrentPauseMenuWidget = CreateWidget<USimPetPauseMenuWidget>(PlayerController, PauseMenuWidgetClass);
			CurrentPauseMenuWidget->AddToViewport(10);
		}
		
		if (CurrentPauseMenuWidget)
			CurrentPauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
		
		PlayerController->bShowMouseCursor = true;
		
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(CurrentPauseMenuWidget ? CurrentPauseMenuWidget->TakeWidget().ToSharedPtr() : nullptr);
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		if (CurrentPauseMenuWidget)
			CurrentPauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		
		if (CurrentHUDWidget)
			CurrentHUDWidget->SetVisibility(ESlateVisibility::Visible);
		
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
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

		const UInputAction *InteractAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Interact);
		if (InteractAction)
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASimPetPlayer::Input_Interact);
		
		const UInputAction *PauseAction = InputConfigDataAsset->FindNativeInputActionByTag(SimPetGameplayTags::InputTag_Pause);
		if (PauseAction)
			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASimPetPlayer::Input_Pause);
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
	if (!InputActionValue.Get<bool>())
		return;

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
		{
			ISimPetInteractable::Execute_Interact(HitActor, this);
		}
	}
}

void ASimPetPlayer::Input_Pause(const FInputActionValue& InputActionValue)
{
	TogglePauseMenu();
}
