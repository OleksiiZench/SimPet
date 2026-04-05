// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetAnimalStatusWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	void Init(ASimPetAnimal *InAnimal);
	
	void SetDirtyIconVisible(bool bVisible);
	void SetHungryIconVisible(bool bVisible);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage *DirtyIcon;
	
	UPROPERTY(meta = (BindWidget))
	UImage *HungryIcon;
	
	ESlateVisibility GetVisibilityFromBool(bool bVisible) const;
	
private:
	ASimPetAnimal *CacheAnimal;
	
	UFUNCTION()
	void HandleDied();
	
	UFUNCTION()
	void HandleGotDirty();
	
	UFUNCTION()
	void HandleGotHungry();
	
	UFUNCTION()
	void HandleWasteCleaned();
};
