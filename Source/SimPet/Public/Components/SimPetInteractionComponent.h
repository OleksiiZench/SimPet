// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetInteractionComponent.generated.h"

class ASimPetItem;
class USceneComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USimPetInteractionComponent();
	
	AActor *DoInteractionTrace();
	
	bool TryUseEquippedItemOn(AActor *TargetActor);
	void TakeOrDropOrSwapItem(AActor *Actor);
	void SetHoldPoint(USceneComponent *InHoldPoint);
	
private:
	UPROPERTY()
	ASimPetItem *CachedTakenItem;
	
	UPROPERTY()
	USceneComponent *HoldPoint;
	
	TEnumAsByte<ETraceTypeQuery> InteractionTraceChanel;
	
	void TakeItem(ASimPetItem *Item);
	void DropItem(ASimPetItem *Item);
	FAttachmentTransformRules GetRuleForAttachingItems();
};
