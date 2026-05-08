// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetNotificationWidget.generated.h"

class UBorder;
class UTextBlock;
class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetNotificationWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SimPet|UI|Notification")
	void ShowNotification(bool bIsSuccess, const FString &Message);
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UBorder *NotificationBorder;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *MessageText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation *SlideAnim;
#pragma endregion
};
