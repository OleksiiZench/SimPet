// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"

#include "SimPetFeedSpawnTrigger.generated.h"

class ASimPetAnimalFeed;
class ASimPetSpawnPoint;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetFeedSpawnTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnZoneEntered(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASimPetAnimalFeed> SpawnFeedClass;
};
