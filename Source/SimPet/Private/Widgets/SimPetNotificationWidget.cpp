// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetNotificationWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void USimPetNotificationWidget::ShowNotification(bool bIsSuccess, const FString &Message)
{
	if (MessageText)
	{
		MessageText->SetText(FText::FromString(Message));
		
		FLinearColor TextColor = FLinearColor::Black;
		MessageText->SetColorAndOpacity(TextColor);
	}
	
	if (NotificationBorder)
	{
		constexpr FLinearColor GreenColor = FLinearColor(0.0f, 0.5f, 0.0f, 0.8f);
		constexpr FLinearColor RedColor = FLinearColor(0.8f, 0.0f, 0.0f, 0.8f);
		
		FLinearColor BorderColor = bIsSuccess ? GreenColor : RedColor;
		
		NotificationBorder->SetBrushColor(BorderColor);
	}
	
	if (SlideAnim)
	{
		PlayAnimation(SlideAnim);
	}
	
	// TODO: bIsSuccess буде відповідати за колір Border
}
