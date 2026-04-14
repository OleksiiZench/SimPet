// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"

USimPetPointsTransactionComponent::USimPetPointsTransactionComponent()
{
}

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PointsPerHappyTick = EconomyData->HappyTickReward;
	PointsPerPenalty = EconomyData->PenaltyPoints;
	
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
