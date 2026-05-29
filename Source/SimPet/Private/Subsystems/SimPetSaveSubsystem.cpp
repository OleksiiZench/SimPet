// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Save/SimPetGlobalSave.h"
#include "Subsystems/SimPetUISubsystem.h"

void USimPetSaveSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	CacheSaveGame();
	
	LoadMaxPoints();
	LoadDifficulty();
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

EGameDifficulty USimPetSaveSubsystem::GetDifficulty() const
{
	return Difficulty;
}

void USimPetSaveSubsystem::SaveDifficulty(EGameDifficulty CurrentDifficulty)
{
	Difficulty = CurrentDifficulty;
	
	if (CachedSaveGame)
	{
		CachedSaveGame->Difficulty = CurrentDifficulty;
		UGameplayStatics::SaveGameToSlot(CachedSaveGame, SaveSlotName, 0);
	}
}

void USimPetSaveSubsystem::SaveGame()
{
	if (USimPetUISubsystem *UISubsystem= GetUISubsystem())
		UISubsystem->BroadcastNotification(true, TEXT("Successful save!"));
}

void USimPetSaveSubsystem::CacheSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		CachedSaveGame = Cast<USimPetGlobalSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		CachedSaveGame = Cast<USimPetGlobalSave>(UGameplayStatics::CreateSaveGameObject(USimPetGlobalSave::StaticClass()));
	}
}

USimPetUISubsystem *USimPetSaveSubsystem::GetUISubsystem() const
{
	if (GetWorld())
	{
		return GetWorld()->GetSubsystem<USimPetUISubsystem>();
	}
	
	return nullptr;
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
	if (CachedSaveGame)
	{
		Difficulty = CachedSaveGame->Difficulty;
	}
	else
	{
		Difficulty = EGameDifficulty::Easy;
	}
}
