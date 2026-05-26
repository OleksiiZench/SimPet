// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetSaveSubsystem.h"

#include "SimPetDebugHelper.h"

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CachePlayerState();
	CacheGameDifficulty();
	
	InitializeEconomySettings();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::RegisterAnimalPurchase()
{
	if (CachedPlayerState)
		CachedPlayerState->MarkFirstAnimalBought();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::ConsumePoints(int32 Cost)
{
	if (CachedPlayerState)
		CachedPlayerState->SpendPoints(Cost);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::GeneratePassivePoints()
{
	if (CachedPlayerState)
		CachedPlayerState->AddPoints(PointsPerHappyTick);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::GeneratePenaltyPoints()
{
	if (CachedPlayerState)
		CachedPlayerState->ApplyPenalty(PointsPerPenalty);
}

int32 USimPetPointsTransactionComponent::GetAnimalPrice(ESimPetAnimals AnimalType) const
{
	if (!CachedPlayerState || !CachedPlayerState->HasBoughtFirstAnimal())
		return 0;
	
	if (EconomyData && EconomyData->AnimalPrices.Contains(AnimalType))
	{
		const FSimPetAnimalPriceConfig &AnimalPrices = EconomyData->AnimalPrices[AnimalType];
		
		return AnimalPrices.PriceByDifficulty.FindRef(CachedGameDifficulty);
	}
	
	Debug::PrintError(TEXT("Animal Price not found in EconomyData!"));	
	return 999999;  // Захист від безкоштовного спавну
}

bool USimPetPointsTransactionComponent::CanAfford(int32 Cost) const
{
	if (CachedPlayerState)
		return CachedPlayerState->HasRequiredPoints(Cost);
	
	return false;
}

void USimPetPointsTransactionComponent::InitializeEconomySettings()
{
	if (EconomyData)
	{
		PointsPerHappyTick = EconomyData->HappyTickReward.FindRef(CachedGameDifficulty);
		PointsPerPenalty = EconomyData->PenaltyPoints.FindRef(CachedGameDifficulty);
	}
	else
	{
		Debug::PrintError(TEXT("EconomyData is nullptr"));
	}
}

void USimPetPointsTransactionComponent::CachePlayerState()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	APlayerController* PC = Cast<APlayerController>(World->GetFirstPlayerController());
	if (!PC)
		return;
	
	CachedPlayerState = PC->GetPlayerState<ASimPetPlayerState>();
}

void USimPetPointsTransactionComponent::CacheGameDifficulty()
{
	CachedGameDifficulty = EGameDifficulty::Easy;
	
	if (UWorld *World = GetWorld())
	{
		if (UGameInstance *GI = World->GetGameInstance())
		{
			USimPetSaveSubsystem *SaveSubsystem = GI->GetSubsystem<USimPetSaveSubsystem>();
			if (SaveSubsystem)
				CachedGameDifficulty = SaveSubsystem->GetDifficulty();
		}
	}
}
