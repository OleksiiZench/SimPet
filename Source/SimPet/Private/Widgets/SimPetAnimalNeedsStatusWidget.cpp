// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetAnimalNeedsStatusWidget.h"

#include "Components/Image.h"

#include "Components/Attributes/SimPetNeedsComponent.h"

#include "SimPetDebugHelper.h"

void USimPetAnimalNeedsStatusWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	SetHungryIconVisible(false);
	SetDirtyIconVisible(false);
}

void USimPetAnimalNeedsStatusWidget::Init(USimPetNeedsComponent *InNeedsComponent)
{
	if (InNeedsComponent)
	{
		CacheAnimalNeedsComponent = InNeedsComponent;
		
		BindNeedsEvents();
	}
	else
	{
		Debug::Print(TEXT("Filed to cache InNeedsComponent!"));
	}
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

void USimPetAnimalNeedsStatusWidget::BindNeedsEvents()
{
	if (CacheAnimalNeedsComponent.IsValid())
	{
		CacheAnimalNeedsComponent->OnNeedsDepleted.AddDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleNeedsDepleted);
		CacheAnimalNeedsComponent->OnGotDirty.AddDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotDirty);
		CacheAnimalNeedsComponent->OnGotHungry.AddDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotHungry);
		CacheAnimalNeedsComponent->OnGotClean.AddDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotClean);
		CacheAnimalNeedsComponent->OnHungrySatisfied.AddDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleHungrySatisfied);
	}
}

void USimPetAnimalNeedsStatusWidget::HandleNeedsDepleted()
{
	SetDirtyIconVisible(false);
	SetHungryIconVisible(false);
}

void USimPetAnimalNeedsStatusWidget::HandleGotDirty()
{
	SetDirtyIconVisible(true);
}

void USimPetAnimalNeedsStatusWidget::HandleGotHungry()
{
	SetHungryIconVisible(true);
}

void USimPetAnimalNeedsStatusWidget::HandleGotClean()
{
	SetDirtyIconVisible(false);
}

void USimPetAnimalNeedsStatusWidget::HandleHungrySatisfied()
{
	SetHungryIconVisible(false);
}
