// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SimPetSavable.generated.h"

class USimPetSaveGame;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USimPetSavable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPET_API ISimPetSavable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save System")
	void SaveActorData(USimPetSaveGame* SaveObject);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SimPet|Save System")
	void LoadActorData(USimPetSaveGame *SaveObject);
};
