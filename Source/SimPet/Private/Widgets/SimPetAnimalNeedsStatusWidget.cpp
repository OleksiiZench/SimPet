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
	UnbindNeedsEvents();
	
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

void USimPetAnimalNeedsStatusWidget::Deinit()
{
	UnbindNeedsEvents();
	CacheAnimalNeedsComponent = nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetAnimalNeedsStatusWidget::SetDirtyIconVisible(const bool bVisible)
{	
	const ESlateVisibility NewVisibility = GetVisibilityFromBool(bVisible);
	
	DirtyIcon->SetVisibility(NewVisibility);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetAnimalNeedsStatusWidget::SetHungryIconVisible(const bool bVisible)
{
	const ESlateVisibility NewVisibility = GetVisibilityFromBool(bVisible);
	
	HungryIcon->SetVisibility(NewVisibility);
}

ESlateVisibility USimPetAnimalNeedsStatusWidget::GetVisibilityFromBool(const bool bVisible)
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

void USimPetAnimalNeedsStatusWidget::UnbindNeedsEvents()
{
	if (CacheAnimalNeedsComponent.IsValid())
	{
		CacheAnimalNeedsComponent->OnNeedsDepleted.RemoveDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleNeedsDepleted);
		CacheAnimalNeedsComponent->OnGotDirty.RemoveDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotDirty);
		CacheAnimalNeedsComponent->OnGotHungry.RemoveDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotHungry);
		CacheAnimalNeedsComponent->OnGotClean.RemoveDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleGotClean);
		CacheAnimalNeedsComponent->OnHungrySatisfied.RemoveDynamic(this, &USimPetAnimalNeedsStatusWidget::HandleHungrySatisfied);
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
