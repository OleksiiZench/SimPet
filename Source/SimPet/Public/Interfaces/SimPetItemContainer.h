// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "SimPetItemContainer.generated.h"

class ASimPetItem;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USimPetItemContainer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SIMPET_API ISimPetItemContainer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	bool TryAddItemToContainer(ASimPetItem *Item);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void GetContainerPayloadClasses(TArray<TSubclassOf<ASimPetItem>> &OutPayloadClasses);
};
