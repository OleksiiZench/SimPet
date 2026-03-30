// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "SimPetAutoHidingWidgetComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPET_API USimPetAutoHidingWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	USimPetAutoHidingWidgetComponent();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	float SecondsPerCheck;
	
	float MaxVisibleDistance;
	
	FTimerHandle CheckDistanceTimerHandle;
	
	void StartCheckDistanceTimer();
	void StopCheckDistanceTimer();
	
	void CheckDistanceToPlayer();
};
