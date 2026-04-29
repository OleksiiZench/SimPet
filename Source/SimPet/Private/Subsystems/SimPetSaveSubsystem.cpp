// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Core/SimPetSaveGame.h"

void USimPetSaveSubsystem::CheckAndUpdateMaxPoints(int32 InCurrentPoints)
{
	if (InCurrentPoints > MaxPoints)
	{
		MaxPoints = InCurrentPoints;

		SaveMaxPoints();
	}
}

void USimPetSaveSubsystem::LoadMaxPoints()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		if (USimPetSaveGame *LoadedGame = Cast<USimPetSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
		{
			MaxPoints = LoadedGame->MaxPoints;
		}
	}
	else
	{
		MaxPoints = 0;
	}
}

void USimPetSaveSubsystem::SaveMaxPoints()
{
	USimPetSaveGame *SaveGameInstance = Cast<USimPetSaveGame>(UGameplayStatics::CreateSaveGameObject(USimPetSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		SaveGameInstance->MaxPoints = MaxPoints;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0);
	}
}
