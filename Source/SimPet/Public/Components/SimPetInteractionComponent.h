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
	
	void TryPickUp();
	void TryInteract();
	AActor *DoInteractionTrace(float InteractionRadius = 1.0f, ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility));
	
	bool TryUseEquippedItemOn(AActor *TargetActor);
	void SetHoldPoint(USceneComponent *InHoldPoint);
	
	FSimPetItemSaveData GetSaveDataEquippedItem() const;
	void ApplyLoadedDataOfEquippedItem(const FSimPetItemSaveData &ItemSaveData);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Collision")
	TEnumAsByte<ETraceTypeQuery> PickUpTraceChannel;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Collision")
	TEnumAsByte<ETraceTypeQuery> InteractTraceChannel;
	
private:
	UPROPERTY()
	ASimPetItem *CachedTakenItem;
	
	UPROPERTY()
	USceneComponent *HoldPoint;
	
	void TakeItem(ASimPetItem *Item);
	void DropItem(ASimPetItem *Item);
	
	static FAttachmentTransformRules GetRuleForAttachingItems();
};
