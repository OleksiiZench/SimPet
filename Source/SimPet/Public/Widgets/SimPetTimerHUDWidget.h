// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetTimerHUDWidget.generated.h"

class UTextBlock;
class USimPetTimeSubsystem;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetTimerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TimerText;
#pragma endregion
	
private:
	UPROPERTY()
	USimPetTimeSubsystem *CachedTimeSubsystem;
	
	UFUNCTION()
	void UpdateTimerDisplay(int32 CurrentTimeSeconds);
	
	void CacheTimeSubsystem();
	
	void BindDelegate();
	void UnbindDelegate();
};
