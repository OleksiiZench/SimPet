// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetPointsTransactionComponent.generated.h"

class ASimPetPlayerState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetPointsTransactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USimPetPointsTransactionComponent();
	
	virtual void BeginPlay() override;
	
	void GeneratePassivePoints();
	void GeneratePenaltyPoints();
	
private:
	int32 PointsPerHappyTick;
	int32 PointsPerPenalty;
	ASimPetPlayerState *PlayerState;
	
	void CachePlayerState();
};
