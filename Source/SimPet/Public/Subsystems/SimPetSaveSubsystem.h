// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SimPetSaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetSaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void CheckAndUpdateMaxPoints(int32 InCurrentPoints);
	int32 GetMaxPoints();
	
private:
	const FString SaveSlotName = TEXT("SimPet_SaveSlot");
	int32 MaxPoints;
	
	void LoadMaxPoints();
	void SaveMaxPoints();
};
