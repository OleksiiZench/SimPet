// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetAnimalNeedsStatusWidget.generated.h"

class UImage;
class USimPetNeedsComponent;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetAnimalNeedsStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	void Init(USimPetNeedsComponent *InNeedsComponent);
	
	void SetDirtyIconVisible(bool bVisible);
	void SetHungryIconVisible(bool bVisible);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage *DirtyIcon;
	
	UPROPERTY(meta = (BindWidget))
	UImage *HungryIcon;
	
	ESlateVisibility GetVisibilityFromBool(bool bVisible) const;
	
private:
	TWeakObjectPtr<USimPetNeedsComponent> CacheAnimalNeedsComponent;
	
	void BindNeedsEvents();
	
	UFUNCTION()
	void HandleNeedsDepleted();
	
	UFUNCTION()
	void HandleGotDirty();
	
	UFUNCTION()
	void HandleGotHungry();
	
	UFUNCTION()
	void HandleGotClean();
	
	UFUNCTION()
	void HandleHungrySatisfied();
};
