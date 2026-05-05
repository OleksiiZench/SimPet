// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/SimPetInteractable.h"

#include "SimPetFabricator.generated.h"

class ASimPetAnimal;
class USimPetPointsTransactionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPurchaseAttemptResultSignature, bool, bIsSuccess, const FString&, Message);

UCLASS()
class SIMPET_API ASimPetFabricator : public AActor, public ISimPetInteractable
{
	GENERATED_BODY()
	
public:
	ASimPetFabricator();
	
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	UPROPERTY(BlueprintAssignable)
	FOnPurchaseAttemptResultSignature OnPurchaseAttemptResult;
	
	UFUNCTION(BlueprintCallable, Category = "Fabricator")
	bool AttemptBuyAnimal(ESimPetAnimals AnimalType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USimPetFabricatorWidget> FabricatorWidgetClass;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Components", meta = (AllowPrivateAccess = "true"))
	USimPetPointsTransactionComponent *PointsTransactionComponent;
	
	void BroadcastResultAnimalPurchase(bool bIsSuccess, const FString &Message);
};
