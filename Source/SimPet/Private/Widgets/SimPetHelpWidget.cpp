// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetHelpWidget.h"

#include "Input/Reply.h"

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
