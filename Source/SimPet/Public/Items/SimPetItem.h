// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"

#include "SimPetItem.generated.h"

UCLASS()
class SIMPET_API ASimPetItem : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	ASimPetItem();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GameplayTags;
};
