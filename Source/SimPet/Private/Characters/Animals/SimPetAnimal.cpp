// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Characters/Animals/SimPetAnimal.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

#include "Components/SimPetPointsTransactionComponent.h"
#include "Widgets/SimPetAnimalNeedsStatusWidget.h"
#include "Components/Attributes/SimPetNeedsComponent.h"
#include "Components/Widgets/SimPetAutoHidingWidgetComponent.h"
#include "Items/SimPetAnimalWaste.h"
#include "Characters/SimPetPlayer.h"
#include "AI/SimPetAIController.h"
#include "DataAsset/SimPetAnimalConfigData.h"

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
	
	SetDifficultyNeeds();
	
	InitializeAnimalStatusWidget();
	
	BindNeedsEvents();
}

void ASimPetAnimal::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindNeedsEvents();
	ClearWasteBindings();
	
	Super::EndPlay(EndPlayReason);
}

void ASimPetAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AnimateLegs(DeltaTime, GetGameTimeSinceCreation());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetAnimal::FeedAnimal()
{
	if (NeedsComponent)
	{
		NeedsComponent->Feed();
	}
}

void ASimPetAnimal::ApplyForestState()
{
	if (NeedsComponent)
		NeedsComponent->DisableNeedsAndBecomeHappy();
	
	UpdateBlackboardForForest();
}

void ASimPetAnimal::ApplyOwnerState()
{
	if (NeedsComponent)
		NeedsComponent->EnableNeeds();
	
	UpdateBlackboardForOwner();
}

ESimPetAnimalState ASimPetAnimal::GetAnimalState() const
{
	return NeedsComponent->GetAnimalState();
}

void ASimPetAnimal::AnimateLegs(float DeltaTime, float CurrentTime)
{
	// 1. Отримуємо поточну швидкість
	float CurrentVelocity = GetVelocity().Size();
	

	
	for (int32 i = 0; i < SpawnedLegs.Num(); i++)
	{
		constexpr float ReturnSpeed = 10.0f;  // Швидкість повернення ніг у вихідне положення
		
		UStaticMeshComponent *Leg = SpawnedLegs[i];
		if (!Leg)
			continue;
			
		FString LegSocketName = LegsSockets[i].ToString();
		FRotator CurrentRotation = Leg->GetRelativeRotation();
		
		float TargetPitch = 0.0f;
	 	
		// 2. Рахуємо синусоїду при русі або польоті
		if (CurrentVelocity > 0.0f || MovementComponent->IsFlying())
		{
			constexpr float WalkSpeed = 10.0f;  // Швидкість ходьби, частота кроків
			bool bIsRightLeg = LegSocketName.Contains(TEXT("Right"));
			float PhaseOffset = bIsRightLeg ? UE_PI : 0.0f;
			
			TargetPitch = FMath::Sin((CurrentTime * WalkSpeed) + PhaseOffset) * LegLiftAngle;
		}	
		
		// 3. Інтерполяція
		float NewPitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetPitch, DeltaTime, ReturnSpeed);
		
		Leg->SetRelativeRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));
	}
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

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetAnimal::HandleHappyTick()
{
	GeneratePointsIfAnimalIsHappy();
}

void ASimPetAnimal::HandleNeedsDepleted()
{
	Die();
}

void ASimPetAnimal::HandleGotDirty()
{
	SpawnAnimalWaste();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetAnimal::HandleWasteCleaned()
{
	CleanAnimal();
}

void ASimPetAnimal::SetDifficultyNeeds()
{
	if (AnimalConfigAsset && NeedsComponent)
	{
		// TODO: Реалізувати діставання складності з SaveSubsystem
		int32 CurrentDifficultyIndex = 0;
		
		FSimPetNeedsConfig SelectedConfig;
		
		if (CurrentDifficultyIndex == 1)
		{
			SelectedConfig = AnimalConfigAsset->NormalConfig;
		}
		else if (CurrentDifficultyIndex == 2)
		{
			SelectedConfig = AnimalConfigAsset->HardConfig;
		}
		else
		{
			SelectedConfig = AnimalConfigAsset->EasyConfig;
		}
		
		NeedsComponent->InitializeNeeds(SelectedConfig);
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

void ASimPetAnimal::InitializeAnimalStatusWidget() const
{
	USimPetAnimalNeedsStatusWidget *AnimalStatusWidget = Cast<USimPetAnimalNeedsStatusWidget>(StatusWidgetComponent->GetUserWidgetObject());
	if (AnimalStatusWidget)
	{
		AnimalStatusWidget->Init(NeedsComponent);
	}
}

void ASimPetAnimal::DeinitializeAnimalStatusWidget() const
{
	USimPetAnimalNeedsStatusWidget *AnimalStatusWidget = Cast<USimPetAnimalNeedsStatusWidget>(StatusWidgetComponent->GetUserWidgetObject());
	if (AnimalStatusWidget)
	{
		AnimalStatusWidget->Deinit();
	}
}

void ASimPetAnimal::BindNeedsEvents()
{
	if (NeedsComponent)
	{
		NeedsComponent->OnHappyTick.AddDynamic(this, &ASimPetAnimal::HandleHappyTick);
		NeedsComponent->OnNeedsDepleted.AddDynamic(this, &ASimPetAnimal::HandleNeedsDepleted);
		NeedsComponent->OnGotDirty.AddDynamic(this, &ASimPetAnimal::HandleGotDirty);
	}
}

void ASimPetAnimal::UnbindNeedsEvents()
{
	if (NeedsComponent)
	{
		NeedsComponent->OnHappyTick.RemoveDynamic(this, &ASimPetAnimal::HandleHappyTick);
		NeedsComponent->OnNeedsDepleted.RemoveDynamic(this, &ASimPetAnimal::HandleNeedsDepleted);
		NeedsComponent->OnGotDirty.RemoveDynamic(this, &ASimPetAnimal::HandleGotDirty);
	}
}

void ASimPetAnimal::SpawnAnimalWaste()
{
	if (!WasteClass || !GetWorld())
		return;
	
	FVector SpawnLocation = GetLocationAboveGround();
	
	ASimPetAnimalWaste *SpawnedAnimalWaste = GetWorld()->SpawnActor<ASimPetAnimalWaste>(WasteClass, SpawnLocation, FRotator::ZeroRotator);
	if (SpawnedAnimalWaste)
	{
		SpawnedAnimalWaste->OnWasteCleaned.AddDynamic(this, &ASimPetAnimal::HandleWasteCleaned);
		TrackedWastes.Add(SpawnedAnimalWaste);
	}
}

FVector ASimPetAnimal::GetLocationAboveGround() const
{
	FVector StartTraceLocation = GetActorLocation();
	FVector EndTraceLocation = StartTraceLocation - FVector(0.0f, 0.0f, 500.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WasteSpawnTrace")), true, this);
	
	bool bHitGround = GetWorld()->LineTraceSingleByChannel(HitResult, StartTraceLocation, EndTraceLocation, ECollisionChannel::ECC_Visibility, TraceParams);
	
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

void ASimPetAnimal::ClearWasteBindings()
{
	for (ASimPetAnimalWaste *Waste : TrackedWastes)
	{
		if (IsValid(Waste))
		{
			Waste->OnWasteCleaned.RemoveDynamic(this, &ASimPetAnimal::HandleWasteCleaned);
		}
	}
	
	TrackedWastes.Empty();
}

void ASimPetAnimal::CleanAnimal() const
{
	if (NeedsComponent)
	{
		NeedsComponent->Wash();
	}
}

void ASimPetAnimal::Die()
{
	if (AController *AnimalController = GetController())
	{
		AnimalController->Destroy();
	}
	
	DeinitializeAnimalStatusWidget();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ScatterMeshParts();
	
	SetLifeSpan(4.0f);
	
	GeneratePenaltyPointsWhenAnimalDied();
	
	OnAnimalDied.Broadcast(this);
}

void ASimPetAnimal::ScatterMeshParts() const
{
	TArray<UStaticMeshComponent *> AllMeshes;
	GetComponents<UStaticMeshComponent>(AllMeshes);
	
	for (UStaticMeshComponent *MeshPart : AllMeshes)
	{
		MeshPart->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		MeshPart->SetCollisionProfileName(TEXT("PhysicsActor"));
		MeshPart->SetSimulatePhysics(true);
		
		FVector RandomKick = FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), FMath::RandRange(100, 600));
		MeshPart->AddImpulse(RandomKick, NAME_None, true);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetAnimal::UpdateBlackboardForForest()
{
	SetBlackboardParam(3000.0f, 3000.0f, 3000.0f, true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetAnimal::UpdateBlackboardForOwner()
{
	SetBlackboardParam(300.0f, 300.0f, 350.0f, true);
}

void ASimPetAnimal::SetBlackboardParam(float BaseRadius, float FlyRadius, float ZigZagRadius, bool bChangeHomeLocation) const
{
	ASimPetAIController *AIController = Cast<ASimPetAIController>(GetController());
	if (!AIController)
		return;
	
	UBlackboardComponent *BBComponent = AIController->GetBlackboardComponent();
	if (!BBComponent)
		return;
	
	BBComponent->SetValueAsFloat(FName("BaseRadiusAroundHome"), BaseRadius);
	BBComponent->SetValueAsFloat(FName("FlyRadiusAroundHome"), FlyRadius);
	BBComponent->SetValueAsFloat(FName("ZigZagRadiusAroundHome"), ZigZagRadius);
	
	if (bChangeHomeLocation)
		BBComponent->SetValueAsVector(FName("HomeLocation"), GetActorLocation());
	
	AIController->StopMovement();
	
	if (UBrainComponent *BrainComponent = AIController->GetBrainComponent())
	{
		BrainComponent->RestartLogic();
	}
}

void ASimPetAnimal::GeneratePointsIfAnimalIsHappy() const
{
	PointsTransactionComponent->GeneratePassivePoints();
}

void ASimPetAnimal::GeneratePenaltyPointsWhenAnimalDied() const
{
	PointsTransactionComponent->GeneratePenaltyPoints();
}
