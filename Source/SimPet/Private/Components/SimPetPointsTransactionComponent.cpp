// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"

#include "SimPetDebugHelper.h"

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PointsPerHappyTick = EconomyData->HappyTickReward;
	PointsPerPenalty = EconomyData->PenaltyPoints;
	
	CachePlayerState();
}

int32 USimPetPointsTransactionComponent::GetAnimalPrice(ESimPetAnimals AnimalType)
{
	if (!PlayerState->HasBoughtFirstAnimal())
		return 0;
	
	if (EconomyData && EconomyData->AnimalPrices.Contains(AnimalType))
	{
		return EconomyData->AnimalPrices[AnimalType];
	}
	
	Debug::PrintError(TEXT("Animal Price not found in EconomyData!"));	
	return 999999;  // Захист від безкоштовного спавну
}

void USimPetPointsTransactionComponent::RegisterAnimalPurchase()
{
	PlayerState->MarkFirstAnimalBought();
}

bool USimPetPointsTransactionComponent::CanAfford(int32 Cost)
{
	if (PlayerState)
		return PlayerState->HasRequiredPoints(Cost);
	
	return false;
}

void USimPetPointsTransactionComponent::ConsumePoints(int32 Cost)
{
	if (PlayerState)
		PlayerState->SpendPoints(Cost);
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
