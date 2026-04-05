// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetAnimalNeedsStatusWidget.h"

#include "Components/Image.h"

void USimPetAnimalNeedsStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetHungryIconVisible(false);
	SetDirtyIconVisible(false);
}

void USimPetAnimalNeedsStatusWidget::Init(USimPetNeedsComponent *InAnimal)
{
	if (InAnimal)
		CacheAnimalNeedsComponent = InAnimal;
}

void USimPetAnimalNeedsStatusWidget::SetDirtyIconVisible(bool bVisible)
{	
	ESlateVisibility Visibility = GetVisibilityFromBool(bVisible);
	
	DirtyIcon->SetVisibility(Visibility);
}

void USimPetAnimalNeedsStatusWidget::SetHungryIconVisible(bool bVisible)
{
	ESlateVisibility Visibility = GetVisibilityFromBool(bVisible);
	
	HungryIcon->SetVisibility(Visibility);
}

ESlateVisibility USimPetAnimalNeedsStatusWidget::GetVisibilityFromBool(bool bVisible) const
{
	ESlateVisibility Visibility = ESlateVisibility::Visible;
	
	if (!bVisible)
		Visibility = ESlateVisibility::Collapsed;
	
	return Visibility;
}

