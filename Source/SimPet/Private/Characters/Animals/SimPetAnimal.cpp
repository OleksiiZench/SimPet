// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetAnimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

#include "Components/SimPetPointsTransactionComponent.h"
#include "Widgets/SimPetAnimalStatusWidget.h"

#include "SimPetDebugHelper.h"

ASimPetAnimal::ASimPetAnimal()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. State
	bIsClean = true;
	MealPerDay = 0;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	AnimalState = ESimPetAnimalState::Happy;

	// 2. Config
	GameSpeed = 0.5f;
	TiredThresholdHours = 8.0f;
	DeathThresholdHours = 24.0f;
	DirtyThresholdHours = 24.0f;

	SetupBody();

	// 4. AI
	bUseCustomHomeLocation = false;
	
	SetupComponent();
}

void ASimPetAnimal::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

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

void ASimPetAnimal::BeginPlay()
{
	Super::BeginPlay();
	
	CacheAnimalStatusWidget();
	
	StartMetabolismTimer();
}

void ASimPetAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AnimateLegs(DeltaTime, GetGameTimeSinceCreation());
}

void ASimPetAnimal::Feed()
{
	TimeSinceLastMeal = 0.0f;
	MealPerDay++;

	Debug::Print("Animal ate.");

	UpdateAnimalState();
}

void ASimPetAnimal::Wash()
{
	TimeSinceLastClean = 0.0f;
	bIsClean = true;

	Debug::Print("Animal is clean now!");

	UpdateAnimalState();
}

ESimPetAnimalState ASimPetAnimal::GetAnimalState()
{
	return AnimalState;
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

void ASimPetAnimal::Die()
{
	AnimalState = ESimPetAnimalState::Dead;
	PointsTransactionComponent->GeneratePenaltyPoints();
	
	StopMetabolismTimer();

	Debug::Print(TEXT("The animal died"));

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
}

void ASimPetAnimal::BecomeDirty()
{
	bIsClean = false;
	
	AnimalStatusWidget->SetDirtyIconVisible(true);

	Debug::Print(TEXT("The animal got dirty"));
}

void ASimPetAnimal::SetupBody()
{
	EyesCount = 0;
	LegsCount = 0;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(GetRootComponent());

	HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
	HeadMesh->SetupAttachment(BodyMesh);

	TailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TailMesh"));
	TailMesh->SetupAttachment(BodyMesh);
}

void ASimPetAnimal::SetupComponent()
{
	MovementComponent = GetCharacterMovement();
	
	PointsTransactionComponent = CreateDefaultSubobject<USimPetPointsTransactionComponent>(TEXT("PointsTransactionComponent"));
	
	StatusWidhetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidhetComponent"));
	StatusWidhetComponent->SetupAttachment(GetRootComponent());
	StatusWidhetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	StatusWidhetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	StatusWidhetComponent->SetDrawSize(FVector2D(150.0f, 50.0f));
}

void ASimPetAnimal::CacheAnimalStatusWidget()
{
	if (!AnimalStatusWidget)
	{
		AnimalStatusWidget = Cast<USimPetAnimalStatusWidget>(StatusWidhetComponent->GetUserWidgetObject());
	}
}

void ASimPetAnimal::OnMetabolismTick()
{
	if (AnimalState == ESimPetAnimalState::Dead)
	{
		return;
	}

	TimeSinceLastMeal++;
	TimeSinceLastClean++;

	CheckPhysicalAnimalState();
	UpdateAnimalState();
	
	GeneratePointsIfAnimalIsHappy();
}

void ASimPetAnimal::CheckPhysicalAnimalState()
{
	if (TimeSinceLastMeal >= DeathThresholdHours)
	{
		Die();
		return;
	}
	
	if (bIsClean && TimeSinceLastClean >= DirtyThresholdHours)
		BecomeDirty();
}

void ASimPetAnimal::UpdateAnimalState()
{
	if (AnimalState == ESimPetAnimalState::Dead)
		return;
	
	if (TimeSinceLastMeal >= TiredThresholdHours || !bIsClean)
	{
		if (AnimalState != ESimPetAnimalState::Tired)
		{
			AnimalState = ESimPetAnimalState::Tired;
			
			if (AnimalStatusWidget)
				AnimalStatusWidget->SetHungryIconVisible(true);
			
			Debug::Print("Animal state changed to Tired");
		}
		
		return;
	}
	
	if (AnimalState != ESimPetAnimalState::Happy)
	{
		AnimalState = ESimPetAnimalState::Happy;
		Debug::Print("Animal state changed to Happy");
	}
}

void ASimPetAnimal::GeneratePointsIfAnimalIsHappy()
{
	if (AnimalState == ESimPetAnimalState::Happy)
		PointsTransactionComponent->GeneratePassivePoints();
}

void ASimPetAnimal::StartMetabolismTimer()
{
	GetWorld()->GetTimerManager().SetTimer(MetabolismTimerHandle, this, &ASimPetAnimal::OnMetabolismTick, GameSpeed, true);
}

void ASimPetAnimal::StopMetabolismTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(MetabolismTimerHandle);
}
