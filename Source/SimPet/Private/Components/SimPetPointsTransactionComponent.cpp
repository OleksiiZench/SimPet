// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"

USimPetPointsTransactionComponent::USimPetPointsTransactionComponent()
{
	PointsPerHappyTick = 5;
	PointsPerPenalty = 100;
}

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CachePlayerState();
}

void USimPetPointsTransactionComponent::GeneratePassivePoints()
{
	if (PlayerState)
		PlayerState->AddPoints(PointsPerHappyTick);
}

void USimPetPointsTransactionComponent::GeneratePenaltyPoints()
{
	if (PlayerState)
		PlayerState->ApplyPenalty(PointsPerPenalty);
}

void USimPetPointsTransactionComponent::CachePlayerState()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	APlayerController* PC = Cast<APlayerController>(World->GetFirstPlayerController());
	if (!PC)
		return;
	
	PlayerState = PC->GetPlayerState<ASimPetPlayerState>();
}
