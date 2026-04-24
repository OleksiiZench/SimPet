// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Core/SimPetPlayerState.h"

ASimPetPlayerState::ASimPetPlayerState()
{
	bHasBoughtFirstAnimal = false;
}

bool ASimPetPlayerState::HasRequiredPoints(int32 Points)
{
	if (CurrentPoints < Points)
		return false;

	return true;
}

void ASimPetPlayerState::SpendPoints(int Points)
{
	CurrentPoints -= Points;
	
	OnPointsChanged.Broadcast(CurrentPoints);
}

void ASimPetPlayerState::AddPoints(int Points)
{
	CurrentPoints += Points;
	
	OnPointsChanged.Broadcast(CurrentPoints);
}

void ASimPetPlayerState::ApplyPenalty(int32 PenaltyAmount)
{
	CurrentPoints -= PenaltyAmount;
	
	OnPointsChanged.Broadcast(CurrentPoints);
}

bool ASimPetPlayerState::HasBoughtFirstAnimal() const
{
	return bHasBoughtFirstAnimal;
}

void ASimPetPlayerState::MarkFirstAnimalBought()
{
	bHasBoughtFirstAnimal = true;
}

int32 ASimPetPlayerState::GetCurrentPoints() const
{
	return CurrentPoints;
}
