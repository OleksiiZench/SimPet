// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetMainLevelGameMode.h"

#include "Characters/SimPetPlayer.h"
#include "Subsystems/SimPetItemSubsystem.h"
#include "Subsystems/SimPetSaveSubsystem.h"

ASimPetMainLevelGameMode::ASimPetMainLevelGameMode()
{
	DefaultPawnClass = ASimPetPlayer::StaticClass();
}

void ASimPetMainLevelGameMode::StartPlay()
{
	Super::StartPlay();
	
	USimPetSaveSubsystem *SaveSubsystem = GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	if (SaveSubsystem)
	{
		if (SaveSubsystem->IsLoadGameRequested())
		{
			SaveSubsystem->LoadGame();
			SaveSubsystem->ClearLoadGameRequested();
		}
	}
}

void ASimPetMainLevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnFeed();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetMainLevelGameMode::SpawnFeed()
{
	USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
	if (ItemSubsystem == nullptr)
		return;
	
	ItemSubsystem->SpawnFeedOnRelevantPoints();
}
