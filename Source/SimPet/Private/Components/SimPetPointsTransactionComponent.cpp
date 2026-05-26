// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetSaveSubsystem.h"

#include "SimPetDebugHelper.h"

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeEconomySettings();
	
	CachePlayerState();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::RegisterAnimalPurchase()
{
	PlayerState->MarkFirstAnimalBought();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::ConsumePoints(int32 Cost)
{
	if (PlayerState)
		PlayerState->SpendPoints(Cost);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::GeneratePassivePoints()
{
	if (PlayerState)
		PlayerState->AddPoints(PointsPerHappyTick);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetPointsTransactionComponent::GeneratePenaltyPoints()
{
	if (PlayerState)
		PlayerState->ApplyPenalty(PointsPerPenalty);
}

int32 USimPetPointsTransactionComponent::GetAnimalPrice(ESimPetAnimals AnimalType) const
{
	if (!PlayerState->HasBoughtFirstAnimal())
		return 0;
	
	if (EconomyData && EconomyData->AnimalPrices.Contains(AnimalType))
	{
		const FSimPetAnimalPriceConfig &AnimalPrices = EconomyData->AnimalPrices[AnimalType];
		
		EGameDifficulty GameDifficulty = GetDifficulty();
		
		return AnimalPrices.PriceByDifficulty.FindRef(GameDifficulty);
	}
	
	Debug::PrintError(TEXT("Animal Price not found in EconomyData!"));	
	return 999999;  // Захист від безкоштовного спавну
}

bool USimPetPointsTransactionComponent::CanAfford(int32 Cost) const
{
	if (PlayerState)
		return PlayerState->HasRequiredPoints(Cost);
	
	return false;
}

void USimPetPointsTransactionComponent::InitializeEconomySettings()
{
	if (EconomyData)
	{
		PointsPerHappyTick = EconomyData->HappyTickReward;
		PointsPerPenalty = EconomyData->PenaltyPoints;
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
	
	PlayerState = PC->GetPlayerState<ASimPetPlayerState>();
}

EGameDifficulty USimPetPointsTransactionComponent::GetDifficulty() const
{
	if (UWorld *World = GetWorld())
	{
		if (UGameInstance *GI = World->GetGameInstance())
		{
			USimPetSaveSubsystem *SaveSubsystem = GI->GetSubsystem<USimPetSaveSubsystem>();
			if (SaveSubsystem)
				return SaveSubsystem->GetDifficulty();
		}
	}
	
	return EGameDifficulty::Easy;
}
