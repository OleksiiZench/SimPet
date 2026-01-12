// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetMainLevelGameMode.h"

#include "Characters/SimPetPlayer.h"

ASimPetMainLevelGameMode::ASimPetMainLevelGameMode()
{
	DefaultPawnClass = ASimPetPlayer::StaticClass();
}
