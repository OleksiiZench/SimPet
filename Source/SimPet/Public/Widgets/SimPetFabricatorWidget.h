// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "WorldObjects/SimPetFabricator.h"

#include "SimPetFabricatorWidget.generated.h"

/**
 * Клас UI, який керує вибором тварин
 */
UCLASS()
class SIMPET_API USimPetFabricatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void InitializeFabricator(ASimPetFabricator *InFabricator);
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void OnAnimalSelected(ESimPetAnimals AnimalType);
	
protected:
	TWeakObjectPtr<ASimPetFabricator> LinkedFabricator;  // Посилання на фабрикатор який відкрив цей UI
};
