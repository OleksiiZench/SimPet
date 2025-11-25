// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "GameModes/SimPetBaseGameMode.h"

#include "SimPetDebugHelper.h"

ASimPetBaseGameMode::ASimPetBaseGameMode()
{
	TotalScore = 0;
}

void ASimPetBaseGameMode::AddScore(int32 PointsToAdd)
{
	TotalScore += PointsToAdd;

	Debug::Print(TEXT("Score Updated"), TotalScore);
}
