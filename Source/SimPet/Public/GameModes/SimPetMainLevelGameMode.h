// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameModes/SimPetBaseGameMode.h"

#include "SimPetMainLevelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetMainLevelGameMode : public ASimPetBaseGameMode
{
	GENERATED_BODY()
	
public:
	ASimPetMainLevelGameMode();
	
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	
private:
	void SpawnFeed();
};
