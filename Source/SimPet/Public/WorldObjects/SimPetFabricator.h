// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/SimPetInteractable.h"

#include "SimPetFabricator.generated.h"

class ASimPetAnimal;
class USimPetPointsTransactionComponent;
class USimPetUISubsystem;
class USimPetAnimalSubsystem;

UCLASS()
class SIMPET_API ASimPetFabricator : public AActor, public ISimPetInteractable
{
	GENERATED_BODY()
	
public:
	ASimPetFabricator();
	
protected:
	virtual void BeginPlay() override;
	
public:	
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	UFUNCTION(BlueprintCallable, Category = "SimPet|Fabricator")
	bool AttemptBuyAnimal(ESimPetAnimals AnimalType);
	
	UFUNCTION(BlueprintCallable, Category = "SimPet|Fabricator")
	void MoveAnimalToForest();
	
	UFUNCTION(BlueprintCallable, Category = "SimPet|Fabricator")
	void MoveAnimalToOwner();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USimPetFabricatorWidget> FabricatorWidgetClass;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|Components", meta = (AllowPrivateAccess = "true"))
	USimPetPointsTransactionComponent *PointsTransactionComponent;
	
	UPROPERTY()
	USimPetAnimalSubsystem *CachedAnimalSubsystem;
	
	UPROPERTY()
	USimPetUISubsystem *CachedUISubsystem;
	
	void BroadcastResultNotification(bool bIsSuccess, const FString &Message);
	
	void CacheAnimalSubsystem();
	void CacheUISubsystem();
};
