// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SimPetItemSubsystem.generated.h"

class ASimPetAnimalFeed;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetItemSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void SpawnFeedOnRelevantPoints();
	
private:
	void CacheSpawnFeedClassFromItemSettings();
	
	TSubclassOf<ASimPetAnimalFeed> CachedSpawnFeedClass;
};
