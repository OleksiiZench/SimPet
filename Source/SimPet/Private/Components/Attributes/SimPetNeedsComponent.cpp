// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/Attributes/SimPetNeedsComponent.h"

USimPetNeedsComponent::USimPetNeedsComponent()
{
	GameSpeed = 0.5f;
}

void USimPetNeedsComponent::StartNeedsTimer()
{
	GetWorld()->GetTimerManager().SetTimer(NeedsTimerHandle, this, &USimPetNeedsComponent::OnNeedsTick, GameSpeed, true);
}

void USimPetNeedsComponent::StopNeedsTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(NeedsTimerHandle);
}

void USimPetNeedsComponent::OnNeedsTick()
{
}
