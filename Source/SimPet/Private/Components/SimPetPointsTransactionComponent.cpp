// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/SimPetPointsTransactionComponent.h"

#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetSaveSubsystem.h"

#include "SimPetDebugHelper.h"

void USimPetPointsTransactionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PointsPerHappyTick = EconomyData->HappyTickReward;
	PointsPerPenalty = EconomyData->PenaltyPoints;
	
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
		FSimPetAnimalPriceConfig AnimalPrices = EconomyData->AnimalPrices[AnimalType];
		
		EGameDifficulty GameDifficulty = EGameDifficulty::Easy;
		
		if (UWorld *World = GetWorld())
		{
			if (UGameInstance *GI = World->GetGameInstance())
			{
				USimPetSaveSubsystem *SaveSubsystem = GI->GetSubsystem<USimPetSaveSubsystem>();
				if (SaveSubsystem)
					GameDifficulty = SaveSubsystem->GetDifficulty();
			}
		}
		
		switch (GameDifficulty)
		{
			case EGameDifficulty::Normal: 
				return AnimalPrices.NormalPrice;
			
			case EGameDifficulty::Hard: 
				return AnimalPrices.HardPrice;
			
			default: 
				return AnimalPrices.EasyPrice;
		}
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
