// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "GameplayTagContainer.h"

#include "SimPetInputConfig.generated.h"

/**
 * Structure for binding Tag and Action Asset
 */
USTRUCT(BlueprintType)
struct FSimPetInputActionConfig
{
	GENERATED_BODY()

public:

	bool IsValid() const { return InputTag.IsValid() && InputAction; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	const UInputAction *InputAction = nullptr;
};

/**
 * Main input configuration
 */
UCLASS()
class SIMPET_API USimPetInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	TArray<FSimPetInputActionConfig> NativeInputActions;

	const UInputAction *FindNativeInputActionByTag (const FGameplayTag &InInputTag) const;
};
