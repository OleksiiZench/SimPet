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
	void SetDirtyIconVisible(bool bVisible);
	void SetHungryIconVisible(bool bVisible);
	void SetSadIconVisible(bool bVisible);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UImage *DirtyIcon;
	
	UPROPERTY(meta = (BindWidget))
	UImage *HungryIcon;
	
	UPROPERTY(meta = (BindWidget))
	UImage *SadIcon;
	
	ESlateVisibility GetVisibilityFromBool(bool bVisible) const;
};
