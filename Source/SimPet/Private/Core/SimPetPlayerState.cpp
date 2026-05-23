// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Core/SimPetPlayerState.h"
#include "Subsystems/SimPetSaveSubsystem.h"

ASimPetPlayerState::ASimPetPlayerState()
{
	bHasBoughtFirstAnimal = false;
}

void ASimPetPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	CacheSaveSubsystem();
}

bool ASimPetPlayerState::HasRequiredPoints(int32 Points) const
{
	if (CurrentPoints < Points)
		return false;

	return true;
}

void ASimPetPlayerState::SpendPoints(int Points)
{
	UpdateCurrentPoints(CurrentPoints -= Points);
}

void ASimPetPlayerState::AddPoints(int Points)
{
	UpdateCurrentPoints(CurrentPoints += Points);
}

void ASimPetPlayerState::ApplyPenalty(int32 PenaltyAmount)
{
	UpdateCurrentPoints(CurrentPoints -= PenaltyAmount);
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

void ASimPetPlayerState::UpdateCurrentPoints(int32 NewCurrentPoints)
{
	CurrentPoints = NewCurrentPoints;
	
	OnPointsChanged.Broadcast(CurrentPoints);
	
	if (CachedSaveSubsystem)
	{
		CachedSaveSubsystem->CheckAndUpdateMaxPoints(CurrentPoints);
	}
}

void ASimPetPlayerState::CacheSaveSubsystem()
{
	if (GetWorld())
	{
		CachedSaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	}
}
