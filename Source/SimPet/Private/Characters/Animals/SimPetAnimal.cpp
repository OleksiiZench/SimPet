// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetAnimal.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/SimPetPointsTransactionComponent.h"
#include "Widgets/SimPetAnimalStatusWidget.h"
#include "Components/Attributes/SimPetNeedsComponent.h"
#include "Components/Widgets/SimPetAutoHidingWidgetComponent.h"
#include "Items/SimPetAnimalWaste.h"

#include "SimPetDebugHelper.h"

ASimPetAnimal::ASimPetAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. AI
	bUseCustomHomeLocation = false;
	
	// 2. BodyParts
	EyesCount = 0;
	LegsCount = 0;
	LegLiftAngle = 45.0f;

	// Other
	InitializeBaseBody();
	
	SetupComponents();
}

void ASimPetAnimal::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	RebuildBodyParts();
}

void ASimPetAnimal::BeginPlay()
{
	Super::BeginPlay();
	
	CacheAnimalStatusWidget();
	
	BindNeedsEvents();
	NeedsComponent->StartNeedsTimer();
}

void ASimPetAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AnimateLegs(DeltaTime, GetGameTimeSinceCreation());
}

ESimPetAnimalState ASimPetAnimal::GetAnimalState()
{
	return NeedsComponent->GetAnimalState();
}

void ASimPetAnimal::RebuildBodyParts()
{
	for (UStaticMeshComponent *SpawnedEye : SpawnedEyes)
	{
		if (SpawnedEye)
			SpawnedEye->DestroyComponent();

		SpawnedEye = nullptr;
	}
	SpawnedEyes.Empty();

	for (UStaticMeshComponent *SpawnedLeg : SpawnedLegs)
	{
		if (SpawnedLeg)
			SpawnedLeg->DestroyComponent();

		SpawnedLeg = nullptr;
	}
	SpawnedLegs.Empty();

	if (EyesAsset && EyesSockets.Num() > 0)
	{
		int32 NumEyesToSpawn = FMath::Min(EyesCount, EyesSockets.Num());

		for (int i = 0; i < NumEyesToSpawn; i++)
		{
			UStaticMeshComponent *NewComp = NewObject<UStaticMeshComponent>(this);
			NewComp->SetStaticMesh(EyesAsset);

			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			NewComp->AttachToComponent(HeadMesh, FAttachmentTransformRules::KeepRelativeTransform, EyesSockets[i]);
			NewComp->RegisterComponent();

			SpawnedEyes.Add(NewComp);
		}
	}

	if (LegsAsset && LegsSockets.Num() > 0)
	{
		int32 NumLegsToSpawn = FMath::Min(LegsCount, LegsSockets.Num());

		for (int i = 0; i < NumLegsToSpawn; i++)
		{
			UStaticMeshComponent *NewComp = NewObject<UStaticMeshComponent>(this);
			NewComp->SetStaticMesh(LegsAsset);

			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			NewComp->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform, LegsSockets[i]);
			NewComp->RegisterComponent();

			SpawnedLegs.Add(NewComp);
		}
	}
}

void ASimPetAnimal::AnimateLegs(float DeltaTime, float CurrentTime)
{
	// 1. Отримуємо поточну швидкість
	float CurrentVelocity = GetVelocity().Size();
	
	float WalkSpeed = 10.0f;  // Швидкість ходьби, частота кроків
	float ReturnSpeed = 10.0f;  // Швидкість повернення ніг у вихідне положення
	
	for (int32 i = 0; i < SpawnedLegs.Num(); i++)
	{
		UStaticMeshComponent *Leg = SpawnedLegs[i];
		if (!Leg)
			continue;
			
		FString LegSocketName = LegsSockets[i].ToString();
		FRotator CurrentRotation = Leg->GetRelativeRotation();
		
		float TargetPitch = 0.0f;
	 	
		// 2. Рахуємо синусоїду при русі або польоті
		if (CurrentVelocity > 0.0f || MovementComponent->IsFlying())
		{
			bool bIsRightLeg = LegSocketName.Contains(TEXT("Right"));
			float PhaseOffset = bIsRightLeg ? UE_PI : 0.0f;
			
			TargetPitch = FMath::Sin((CurrentTime * WalkSpeed) + PhaseOffset) * LegLiftAngle;
		}	
		
		// 3. Інтерполяція
		float NewPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, DeltaTime, ReturnSpeed);
		
		Leg->SetRelativeRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));
	}
}

void ASimPetAnimal::InitializeBaseBody()
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetRootComponent());

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(BodyMesh);

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	TailMesh->SetupAttachment(BodyMesh);
}

void ASimPetAnimal::SetupComponents()
{
	MovementComponent = GetCharacterMovement();
	
	NeedsComponent = CreateDefaultSubobject<USimPetNeedsComponent>(TEXT("NeedsComponent"));
	
	PointsTransactionComponent = CreateDefaultSubobject<USimPetPointsTransactionComponent>(TEXT("PointsTransactionComponent"));
	
	StatusWidgetComponent = CreateDefaultSubobject<USimPetAutoHidingWidgetComponent>(TEXT("StatusWidgetComponent"));
	StatusWidgetComponent->SetupAttachment(GetRootComponent());
	StatusWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusWidgetComponent->SetDrawSize(FVector2D(200.0f, 100.0f));
}

void ASimPetAnimal::CacheAnimalStatusWidget()
{
	if (!AnimalStatusWidget)
	{
		AnimalStatusWidget = Cast<USimPetAnimalStatusWidget>(StatusWidgetComponent->GetUserWidgetObject());
	}
}

void ASimPetAnimal::BindNeedsEvents()
{
	NeedsComponent->OnHappyTick.AddDynamic(this, &ASimPetAnimal::HandleHappyTick);
	NeedsComponent->OnDied.AddDynamic(this, &ASimPetAnimal::HandleDied);
	NeedsComponent->OnGotDirty.AddDynamic(this, &ASimPetAnimal::HandleGotDirty);
	NeedsComponent->OnGotHungry.AddDynamic(this, &ASimPetAnimal::HandleGotHungry);
}

void ASimPetAnimal::SpawnAnimalWaste()
{
	if (!WasteClass && !GetWorld())
		return;
	
	FVector SpawnLocation = GetLocationAboveGround();
	
	float SpawnLocationZ = SpawnLocation.Z;
	
	Debug::Print(TEXT("SpawnLocation"), SpawnLocationZ);
	
	ASimPetAnimalWaste *SpawnedAnimalWaste = GetWorld()->SpawnActor<ASimPetAnimalWaste>(WasteClass, SpawnLocation, FRotator::ZeroRotator);

	if (SpawnedAnimalWaste)
	{
		SpawnedAnimalWaste->OnWasteCleaned.AddDynamic(this, &ASimPetAnimal::HandleWasteCleaned);
	}
}

FVector ASimPetAnimal::GetLocationAboveGround()
{
	FVector StartTraceLocation = GetActorLocation();
	FVector EndTraceLocation = StartTraceLocation - FVector(0.0f, 0.0f, 500.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WasteSpawnTrace")), true, this);
	
	bool bHitGround = GetWorld()->LineTraceSingleByChannel(HitResult, StartTraceLocation, EndTraceLocation, ECollisionChannel::ECC_Visibility);
	
	FVector SpawnLocation;
	
	if (bHitGround)
	{
		SpawnLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, 3.0f);
	}
	else
	{
		SpawnLocation = GetActorLocation();
	}
	
	return SpawnLocation;
}

void ASimPetAnimal::HandleHappyTick()
{
	GeneratePointsIfAnimalIsHappy();
}

void ASimPetAnimal::HandleDied()
{
	GeneratePenaltyPointsWhenAnimalDied();
	
	// Також звідси буде йти логіка регдолу, знищення обєкту тварини і тд...
}

void ASimPetAnimal::HandleGotDirty()
{
	if (AnimalStatusWidget)
		AnimalStatusWidget->SetDirtyIconVisible(true);
	
	SpawnAnimalWaste();
}

void ASimPetAnimal::HandleGotHungry()
{
	if (AnimalStatusWidget)
		AnimalStatusWidget->SetHungryIconVisible(true);
}

void ASimPetAnimal::HandleWasteCleaned()
{
	CleanAnimal();
}

void ASimPetAnimal::CleanAnimal()
{
	if (NeedsComponent)
	{
		NeedsComponent->Wash();
		
		if (AnimalStatusWidget)
			AnimalStatusWidget->SetDirtyIconVisible(false);
	}
}

void ASimPetAnimal::GeneratePointsIfAnimalIsHappy()
{
	PointsTransactionComponent->GeneratePassivePoints();
}

void ASimPetAnimal::GeneratePenaltyPointsWhenAnimalDied()
{
	PointsTransactionComponent->GeneratePenaltyPoints();
}
