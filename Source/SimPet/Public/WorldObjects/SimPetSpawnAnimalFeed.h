// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "SimPetSpawnAnimalFeed.generated.h"

class ASimPetAnimalFeed;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetSpawnAnimalFeed : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ASimPetSpawnAnimalFeed();
	
	UFUNCTION()
	void OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASimPetAnimalFeed> SpawnFeedClass;
};
