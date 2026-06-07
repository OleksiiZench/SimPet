// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SimPetTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdatedSignature, int32, CurrentTime);

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetTimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	USimPetTimeSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UPROPERTY()
	FOnTimeUpdatedSignature OnTimeUpdated;
	
	int32 GetCurrentPlayTime() const;
	void RestoreTimeFromSave(int32 SavedTime);
	
private:
	int32 TotalPlayTime;
	FTimerHandle ClockTimerHandle;
	
	UFUNCTION()
	void TickGameTime();
};
