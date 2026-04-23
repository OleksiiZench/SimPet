// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetTimerHUDWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetTimerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TimerText;
#pragma endregion
	
private:
	UFUNCTION()
	void UpdateTimerDisplay();
	
	FTimerHandle ClockTimerHandle;
};
