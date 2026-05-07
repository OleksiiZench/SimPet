// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetNotificationWidget.h"

#include "Components/TextBlock.h"

void USimPetNotificationWidget::ShowNotification(bool bIsSuccess, const FString &Message)
{
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Message));
		
		FLinearColor TextColor = bIsSuccess ? FLinearColor::Green : FLinearColor:: Red;
		MessageText->SetColorAndOpacity(TextColor);
	}
	
	if (SlideAnim)
	{
		PlayAnimation(SlideAnim);
	}
}
