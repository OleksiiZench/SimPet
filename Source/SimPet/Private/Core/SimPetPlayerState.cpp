// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Core/SimPetPlayerState.h"

bool ASimPetPlayerState::SpendPoints(int Points)
{
	if (CurrentPoints < Points)
		return false;
	
	CurrentPoints -= Points;
	
	OnPointsChanged.Broadcast(CurrentPoints);
	
	return true;
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
