// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameplayTagAssetInterface.h"

#include "Interfaces/SimPetSavable.h"

#include "SimPetSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetSpawnPoint : public ATargetPoint, public IGameplayTagAssetInterface, public ISimPetSavable
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer &TagContainer) const override;
	
	//~ Begin ISimPetSavable Interface
	virtual void SaveActorData_Implementation(USimPetSaveGame* SaveObject) override;
	virtual void LoadActorData_Implementation(USimPetSaveGame *SaveObject) override;
	//~ End ISimPetSavable Interface
	
	void AddGameplayTags(const FGameplayTag& Tag);  // Думаю цей метод можна перевизначити з інтерфейсу IGameplayTagAssetInterface
	void RemoveGameplayTags(const FGameplayTag& Tag);
	
	UFUNCTION()
	void HandleFeedPickedUp(ASimPetAnimalFeed *PickedFeed);
	
	UFUNCTION()
	void HandleFeedDestroyed(AActor *DestroyedActor);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTagContainer GameplayTags;
	
private:
	void RegisterSpawnPointInAnimalSubsystem();
	void RegisterSpawnPointInSaveSubsystem();
	void RemoveSpawnPointFromSaveSubsystem();
	void ClearFeedTag();
};
