// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetInteractionComponent.generated.h"

class ASimPetItem;
class USceneComponent;
struct FSimPetItemSaveData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USimPetInteractionComponent();
	
	AActor *DoInteractionTrace(float InteractionRadius = 1.0f);
	
	bool TryUseEquippedItemOn(AActor *TargetActor);
	void TakeOrDropOrSwapItem(AActor *Actor);
	void SetHoldPoint(USceneComponent *InHoldPoint);
	
	FSimPetItemSaveData GetSaveDataEquippedItem() const;
	void ApplyLoadedDataOfEquippedItem(const FSimPetItemSaveData &ItemSaveData);
	
private:
	UPROPERTY()
	ASimPetItem *CachedTakenItem;
	
	UPROPERTY()
	USceneComponent *HoldPoint;
	
	TEnumAsByte<ETraceTypeQuery> InteractionTraceChannel;
	
	void TakeItem(ASimPetItem *Item);
	void DropItem(ASimPetItem *Item);
	
	static FAttachmentTransformRules GetRuleForAttachingItems();
};
