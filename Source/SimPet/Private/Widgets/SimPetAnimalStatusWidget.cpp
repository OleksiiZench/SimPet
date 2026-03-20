// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetAnimalStatusWidget.h"

#include "Components/Image.h"

void USimPetAnimalStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetDirtyIconVisible(false);
	SetHungryIconVisible(false);
	SetSadIconVisible(false);
}

void USimPetAnimalStatusWidget::SetDirtyIconVisible(bool bVisible)
{
	ESlateVisibility Visibility = GetVisibilityFromBool(bVisible);
	
	DirtyIcon->SetVisibility(Visibility);
}

void USimPetAnimalStatusWidget::SetHungryIconVisible(bool bVisible)
{
	ESlateVisibility Visibility = GetVisibilityFromBool(bVisible);
	
	HungryIcon->SetVisibility(Visibility);
}

void USimPetAnimalStatusWidget::SetSadIconVisible(bool bVisible)
{
	ESlateVisibility Visibility = GetVisibilityFromBool(bVisible);
	
	SadIcon->SetVisibility(Visibility);
}

ESlateVisibility USimPetAnimalStatusWidget::GetVisibilityFromBool(bool bVisible) const
{
	ESlateVisibility Visibility = ESlateVisibility::Visible;
	
	if (!bVisible)
		Visibility = ESlateVisibility::Collapsed;
	
	return Visibility;
}
