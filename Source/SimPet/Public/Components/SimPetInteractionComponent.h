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
	
	bool TryUseEquippedItemOn(AActor *TargetActor);
	void TakeOrDropItem(AActor *Actor);
	void SetHoldPoint(USceneComponent *InHoldPoint);
	
private:
	ASimPetItem *CaсhedTakenItem;
	USceneComponent *HoldPoint;
	
	void TakeItem(ASimPetItem *Item);
	void DropItem(ASimPetItem *Item);
	FAttachmentTransformRules GetRuleForAttachingItems();
};
