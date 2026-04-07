// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameplayTagAssetInterface.h"

#include "SimPetSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetSpawnPoint : public ATargetPoint, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;
	
	void AddGameplayTags(const FGameplayTag& Tag);  // Думаю цей метод можна перевизначити з інтерфейсу IGameplayTagAssetInterface
	void RemoveGameplayTags(const FGameplayTag& Tag);
	
	UFUNCTION()
	void HandleFeedPickedUp(ASimPetAnimalFeed *PickedFeed);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
private:
	void RegisterSpawnPointInAnimaSubsystem();
};
