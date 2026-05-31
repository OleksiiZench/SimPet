// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Save/SimPetGlobalSave.h"
#include "Save/SimPetSaveGame.h"
#include "Subsystems/SimPetUISubsystem.h"
#include "Interfaces/SimPetSavable.h"

#include "SimPetDebugHelper.h"

USimPetSaveSubsystem::USimPetSaveSubsystem()
{
	bLoadGameRequested = false;
}

void USimPetSaveSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	CacheGlobalSave();
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
	
	if (CachedGlobalSave)
	{
		CachedGlobalSave->Difficulty = CurrentDifficulty;
		UGameplayStatics::SaveGameToSlot(CachedGlobalSave, GlobalSaveSlotName, 0);
	}
}

void USimPetSaveSubsystem::SaveGame()
{
	CacheSaveGame();
	
	for (AActor *Actor : RegisteredSavableActors)
	{
		ISimPetSavable::Execute_SaveActorData(Actor, CachedSaveGame);
	}
	
	bool bIsSaved = UGameplayStatics::SaveGameToSlot(CachedSaveGame, SaveGameSlotName, 0);
	
	if (USimPetUISubsystem *UISubsystem= GetUISubsystem())
	{
		if (bIsSaved)
			UISubsystem->BroadcastNotification(true, TEXT("Game Saved!"));
		else
			UISubsystem->BroadcastNotification(false, TEXT("Save Error!"));
	}
}

void USimPetSaveSubsystem::LoadGame()
{
	USaveGame *LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	if (LoadedGame != nullptr)
	{
		Debug::Print(TEXT("RegisteredSavableActors.Num() > 0"), RegisteredSavableActors.Num() > 0);
		
		CachedSaveGame = Cast<USimPetSaveGame>(LoadedGame);
		
		for (AActor *Actor : RegisteredSavableActors)
		{
			ISimPetSavable::Execute_LoadActorData(Actor, CachedSaveGame);
		}
	}
}

void USimPetSaveSubsystem::RegisterSavableActor(AActor *ActorToRegister)
{
	if (ActorToRegister && ActorToRegister->Implements<USimPetSavable>())
	{
		RegisteredSavableActors.AddUnique(ActorToRegister);
	}
}

void USimPetSaveSubsystem::UnregisterSavableActor(AActor *ActorToRemove)
{
	if (ActorToRemove)
	{
		RegisteredSavableActors.Remove(ActorToRemove);
	}
}

void USimPetSaveSubsystem::RequestLoadGame()
{
	bLoadGameRequested = true;
}

bool USimPetSaveSubsystem::IsLoadGameRequested()
{
	return bLoadGameRequested;
}

void USimPetSaveSubsystem::ClearLoadGameRequested()
{
	bLoadGameRequested = false;
}

void USimPetSaveSubsystem::CacheGlobalSave()
{
	if (UGameplayStatics::DoesSaveGameExist(GlobalSaveSlotName, 0))
	{
		CachedGlobalSave = Cast<USimPetGlobalSave>(UGameplayStatics::LoadGameFromSlot(GlobalSaveSlotName, 0));
	}
	else
	{
		CachedGlobalSave = Cast<USimPetGlobalSave>(UGameplayStatics::CreateSaveGameObject(USimPetGlobalSave::StaticClass()));
	}
}

void USimPetSaveSubsystem::CacheSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		CachedSaveGame = Cast<USimPetSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	}
	else
	{
		CachedSaveGame = Cast<USimPetSaveGame>(UGameplayStatics::CreateSaveGameObject(USimPetSaveGame::StaticClass()));
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
	if (CachedGlobalSave)
	{
		MaxPoints = CachedGlobalSave->MaxPoints;
	}
	else
	{
		MaxPoints = 0;
	}
}

void USimPetSaveSubsystem::SaveMaxPoints() const
{
	if (CachedGlobalSave)
	{
		CachedGlobalSave->MaxPoints = MaxPoints;
		UGameplayStatics::SaveGameToSlot(CachedGlobalSave, GlobalSaveSlotName, 0);
	}
}

void USimPetSaveSubsystem::LoadDifficulty()
{
	if (CachedGlobalSave)
	{
		Difficulty = CachedGlobalSave->Difficulty;
	}
	else
	{
		Difficulty = EGameDifficulty::Easy;
	}
}
