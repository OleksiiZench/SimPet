// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "WorldObjects/SimPetAnimalSubsystem.h"

#include "SimPetFabricatorWidget.generated.h"

class ASimPetFabricator;
class UTextBlock;
class USimPetPointsTransactionComponent;
class ASimPetPlayerState;

/**
 * Клас UI, який керує вибором тварин
 */
UCLASS()
class SIMPET_API USimPetFabricatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USimPetFabricatorWidget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override;
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void InitializeFabricator(ASimPetFabricator *InFabricator);
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void OnBuyAnimalClicked(ESimPetAnimals AnimalType);
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void OnCloseMenu();
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void MoveAnimalToForest();
	
	UFUNCTION(BlueprintCallable, Category = "SimPet | Widgets")
	void MoveAnimalToOwner();
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *PriceDogText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *PriceCanaryText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *PriceLizardText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *PointsText;
#pragma endregion
	
	TWeakObjectPtr<ASimPetFabricator> LinkedFabricator;  // Посилання на фабрикатор який відкрив цей UI
	
	USimPetAnimalSubsystem *AnimalSubsystem;
	
private:
	TWeakObjectPtr<USimPetPointsTransactionComponent> CachedTransactionComponent;
	ASimPetPlayerState *CachedPlayerState;
	
	UFUNCTION()
	void UpdatePointsText(int32 Points);
	
	void UpdateAnimalPricesUI();
	
	void CachePlayerState();
	void BindToPointsUpdate();
};
