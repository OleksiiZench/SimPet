// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/SimPetInteractable.h"

#include "SimPetFabricator.generated.h"

class ASimPetAnimal;

UENUM(BlueprintType)
enum class ESimPetAnimals : uint8
{
	EA_Canary	UMETA(DisplayName = "Canary"),
	EA_Dog		UMETA(DisplayName = "Dog"),
	EA_Lizard	UMETA(DisplayName = "Lizard"),
};

UCLASS()
class SIMPET_API ASimPetFabricator : public AActor, public ISimPetInteractable
{
	GENERATED_BODY()
	
public:
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	UFUNCTION(BlueprintCallable, Category = "Fabricator")
	void RequestSpawnAnimal(ESimPetAnimals AnimalType);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USimPetFabricatorWidget> FabricatorWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimalSettings")
	TMap<ESimPetAnimals, TSubclassOf<ASimPetAnimal>> AnimalClassMap;
};
