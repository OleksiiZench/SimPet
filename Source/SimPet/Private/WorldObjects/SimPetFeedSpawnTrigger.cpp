// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFeedSpawnTrigger.h"

#include "Components/ShapeComponent.h"

#include "Characters/SimPetPlayer.h"
#include "Subsystems/SimPetItemSubsystem.h"

void ASimPetFeedSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnZoneEntered);
}

void ASimPetFeedSpawnTrigger::OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!OtherActor || !OtherActor->IsA(ASimPetPlayer::StaticClass()))
		return;
	
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem == nullptr)
		return;
	
	ItemSubsystem->SpawnFeedOnRelevantPoints();
}
