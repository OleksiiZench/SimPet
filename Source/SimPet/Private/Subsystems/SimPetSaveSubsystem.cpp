// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Core/SimPetSaveGame.h"

void USimPetSaveSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	CacheSaveGame();
	
	LoadMaxPoints();
}

void USimPetSaveSubsystem::CheckAndUpdateMaxPoints(int32 InCurrentPoints)
{
	if (InCurrentPoints > MaxPoints)
	{
		MaxPoints = InCurrentPoints;

		SaveMaxPoints();
	}
}

int32 USimPetSaveSubsystem::GetMaxPoints() const
{
	return MaxPoints;
}

void USimPetSaveSubsystem::SaveDifficulty(EGameDifficulty CurrentDifficulty) const
{
	if (CachedSaveGame)
	{
		CachedSaveGame->Difficulty = CurrentDifficulty;
		UGameplayStatics::SaveGameToSlot(CachedSaveGame, SaveSlotName, 0);
	}
}

void USimPetSaveSubsystem::CacheSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CachedSaveGame = Cast<USimPetSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		CachedSaveGame = Cast<USimPetSaveGame>(UGameplayStatics::CreateSaveGameObject(USimPetSaveGame::StaticClass()));
	}
}

void USimPetSaveSubsystem::LoadMaxPoints()
{
	if (CachedSaveGame)
	{
		MaxPoints = CachedSaveGame->MaxPoints;
	}
	else
	{
		MaxPoints = 0;
	}
}

void USimPetSaveSubsystem::SaveMaxPoints() const
{
	if (CachedSaveGame)
	{
		CachedSaveGame->MaxPoints = MaxPoints;
		UGameplayStatics::SaveGameToSlot(CachedSaveGame, SaveSlotName, 0);
	}
}

void USimPetSaveSubsystem::LoadDifficulty()
{
}
