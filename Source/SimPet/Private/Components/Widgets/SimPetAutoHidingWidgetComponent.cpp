// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Components/Widgets/SimPetAutoHidingWidgetComponent.h"

#include "Kismet/GameplayStatics.h"

USimPetAutoHidingWidgetComponent::USimPetAutoHidingWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxVisibleDistance = 600.0f;
	
	SecondsPerCheck = 0.2f;
}

void USimPetAutoHidingWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	StartCheckDistanceTimer();
}

void USimPetAutoHidingWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopCheckDistanceTimer();
	
	Super::EndPlay(EndPlayReason);
}

void USimPetAutoHidingWidgetComponent::StartCheckDistanceTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CheckDistanceTimerHandle, this, &USimPetAutoHidingWidgetComponent::CheckDistanceToPlayer, SecondsPerCheck, true);
	}
}

void USimPetAutoHidingWidgetComponent::StopCheckDistanceTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CheckDistanceTimerHandle);
	}
}

void USimPetAutoHidingWidgetComponent::CheckDistanceToPlayer()
{
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (PlayerPawn)
	{		
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FVector MyLocation = GetComponentLocation();
		
		float Distance = FVector::Dist(PlayerLocation, MyLocation);
		
		if (Distance <= MaxVisibleDistance)
		{
			if (!IsVisible())
			{
				SetVisibility(true);
			}
		}
		else
		{
			if (IsVisible())
			{
				SetVisibility(false);
			}
		}
	}
}
