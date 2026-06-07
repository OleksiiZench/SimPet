// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetTimeSubsystem.h"

USimPetTimeSubsystem::USimPetTimeSubsystem()
{
	TotalPlayTime = 0;
}

void USimPetTimeSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	if (GetWorld())
		GetWorld()->GetTimerManager().SetTimer(ClockTimerHandle, this, &USimPetTimeSubsystem::TickGameTime, 1.0f, true);
}

void USimPetTimeSubsystem::Deinitialize()
{
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(ClockTimerHandle);
	
	Super::Deinitialize();
}

int32 USimPetTimeSubsystem::GetCurrentPlayTime() const
{
	return TotalPlayTime;
}

void USimPetTimeSubsystem::RestoreTimeFromSave(int32 SavedTime)
{
	TotalPlayTime = SavedTime;
	
	OnTimeUpdated.Broadcast(TotalPlayTime);
}

void USimPetTimeSubsystem::TickGameTime()
{
	TotalPlayTime += 1;
	
	OnTimeUpdated.Broadcast(TotalPlayTime);
}
