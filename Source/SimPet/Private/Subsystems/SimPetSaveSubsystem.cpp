// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Save/SimPetGlobalSave.h"
#include "Save/SimPetSaveGame.h"
#include "Subsystems/SimPetUISubsystem.h"
#include "Interfaces/SimPetSavable.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "Subsystems/SimPetItemSubsystem.h"
#include "Subsystems/SimPetTimeSubsystem.h"
#include "Core/SimPetPlayerState.h"

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

bool USimPetSaveSubsystem::HasAnySaveGame() const
{
	return UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0);
}

void USimPetSaveSubsystem::SaveGame()
{
	CacheSaveGame();
	
	SaveRegisteredActorsData();
	
	SaveDroppedItemsData();
	SaveAnimalsData();
	SaveTimerData();
	SaveCurrentPointsData();
	
	bool bIsSaved = WriteSaveGameToDisk();
	NotifySaveResult(bIsSaved);
}

void USimPetSaveSubsystem::LoadGame()
{
	ReadSaveGameFromDisk();
	
	RestoreDroppedItemsData();
	RestoreAnimalsData();
	RestoreTimerData();
	RestoreCurrentPointsData();
	
	LoadRegisteredActorsData();
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

bool USimPetSaveSubsystem::IsLoadGameRequested() const
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

void USimPetSaveSubsystem::SaveRegisteredActorsData()
{
	for (AActor *Actor : RegisteredSavableActors)
	{
		ISimPetSavable::Execute_SaveActorData(Actor, CachedSaveGame);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::SaveDroppedItemsData()
{
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem)
		CachedSaveGame->DroppedItemsSaveData = ItemSubsystem->GetDroppedItemsSaveData();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::SaveAnimalsData()
{
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem)
		CachedSaveGame->AnimalsSaveData = AnimalSubsystem->GetAnimalsSaveData();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::SaveTimerData()
{
	USimPetTimeSubsystem *TimeSubsystem = GetWorld()->GetSubsystem<USimPetTimeSubsystem>();
	if (TimeSubsystem)
		CachedSaveGame->PlayTimeSaveData = TimeSubsystem->GetCurrentPlayTime();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::SaveCurrentPointsData()
{
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
		return;
	
	ASimPetPlayerState *PlayerState = PlayerController->GetPlayerState<ASimPetPlayerState>();
	if (PlayerState == nullptr)
		return;
	
	CachedSaveGame->PointsSaveData = PlayerState->GetSaveData();
}

bool USimPetSaveSubsystem::WriteSaveGameToDisk() const
{
	if (!IsValid(CachedSaveGame))
		return false;
	
	return UGameplayStatics::SaveGameToSlot(CachedSaveGame, SaveGameSlotName, 0);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::NotifySaveResult(const bool bIsSuccess)
{
	USimPetUISubsystem *UISubsystem= GetUISubsystem();
	if (UISubsystem == nullptr)
		return;

	if (bIsSuccess)
		UISubsystem->BroadcastNotification(true, TEXT("Game Saved!"));
	else
		UISubsystem->BroadcastNotification(false, TEXT("Save Error!"));
	
}

bool USimPetSaveSubsystem::ReadSaveGameFromDisk()
{
	USaveGame *LoadedGame = UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0);
	if (LoadedGame == nullptr)
		return false;
	
	CachedSaveGame = Cast<USimPetSaveGame>(LoadedGame);
	
	return IsValid(CachedSaveGame);
}

void USimPetSaveSubsystem::LoadRegisteredActorsData()
{
	if (!IsValid(CachedSaveGame))
		return;
	
	for (AActor *Actor : RegisteredSavableActors)
	{
		if (IsValid(Actor))
			ISimPetSavable::Execute_LoadActorData(Actor, CachedSaveGame);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::RestoreDroppedItemsData()
{
	if (!IsValid(CachedSaveGame))
		return;
	
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem)
		ItemSubsystem->RestoreDroppedItemsFromSaveData(CachedSaveGame->DroppedItemsSaveData);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::RestoreAnimalsData()
{
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem)
		AnimalSubsystem->RestoreAnimalsFromSave(CachedSaveGame->AnimalsSaveData);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::RestoreTimerData()
{
	USimPetTimeSubsystem *TimeSubsystem = GetWorld()->GetSubsystem<USimPetTimeSubsystem>();
	if (TimeSubsystem)
		TimeSubsystem->RestoreTimeFromSave(CachedSaveGame->PlayTimeSaveData);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetSaveSubsystem::RestoreCurrentPointsData()
{
	if (!IsValid(CachedSaveGame))
		return;
	
	APlayerController *PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
		return;
	
	ASimPetPlayerState *PlayerState = PlayerController->GetPlayerState<ASimPetPlayerState>();
	if (PlayerState == nullptr)
		return;
	
	PlayerState->RestoreFromSaveData(CachedSaveGame->PointsSaveData);
}
