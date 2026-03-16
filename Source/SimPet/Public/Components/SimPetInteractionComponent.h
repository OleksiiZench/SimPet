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
	AActor *DoInteractionTrace();
	
	bool TryItemInteraction(ASimPetItem *TargetItem);
	void TakeOrDropItem(ASimPetItem *Item);
	void SetHoldPoint(USceneComponent *InHoldPoint);
	
private:
	bool bHandsFull = false;
	ASimPetItem *CashedTakenItem;
	USceneComponent *HoldPoint;
	
	void TakeItem(ASimPetItem *Item);
	void DropItem(ASimPetItem *Item);
	FAttachmentTransformRules GetRuleForAttachingItems();
};
