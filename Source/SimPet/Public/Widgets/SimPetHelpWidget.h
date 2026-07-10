// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetHelpWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHelpClosedSignature);

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetHelpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "SimPet|Events")
	FOnHelpClosedSignature OnHelpClosed;
	
protected:
	virtual FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
