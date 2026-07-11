// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHelpWidget.h"

#include "Input/Reply.h"
#include "Components/TextBlock.h"

void USimPetHelpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetupTextBindings();
}

FReply USimPetHelpWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnHelpClosed.Broadcast();
		
		RemoveFromParent();
		
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply USimPetHelpWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnHelpClosed.Broadcast();
	RemoveFromParent();
	return FReply::Handled();
}

void USimPetHelpWidget::SetupTextBindings() const
{
	if (Text_Welcome)
	{
		Text_Welcome->SetText(NSLOCTEXT("HelpWidget", "Text_Welcome", "Welcome to SimPet"));
	}
	
	if (Text_KeysMeanings)
	{
		Text_KeysMeanings->SetText(NSLOCTEXT("HelpWidget", "Text_KeysMeanings", "Keys and Their Meanings"));
	}
	
	if (Text_ListKeysMeanings)
	{
		Text_ListKeysMeanings->SetText(NSLOCTEXT("HelpWidget", "Text_ListKeysMeanings", 
												 "WASD - walk\n"
												 "Shift - sprint\n"
												 "E - pick up an item\n"
												 "Left mouse button - use an item\n"
												 "Right mouse button - take an item out of your backpack\n"
												 "ESC - pause\n"
												 "F5 - save"));
	}
	
	if (Text_TipWhereToLook)
	{
		Text_TipWhereToLook->SetText(NSLOCTEXT("HelpWidget", "Text_TipWhereToLook", "(You can view this tip in the pause menu)"));
	}
}
