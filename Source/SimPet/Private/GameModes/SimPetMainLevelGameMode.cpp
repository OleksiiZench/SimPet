// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetMainLevelGameMode.h"

#include "GameFramework/GameUserSettings.h"

#include "Characters/SimPetPlayer.h"
#include "Subsystems/SimPetItemSubsystem.h"

ASimPetMainLevelGameMode::ASimPetMainLevelGameMode()
{
	DefaultPawnClass = ASimPetPlayer::StaticClass();
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
