// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ASimPetPlayer;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *TimerText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *DogCountText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *CanaryCountText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *LizardCountText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar *StaminaProgressBar;
#pragma endregion
	
#pragma region Logic
	UFUNCTION()
	void UpdateStaminaBar(float NewValue, float Percent);
	
	UFUNCTION()
	void UpdateAnimalStats();  // Функція для оновлення статистики (за таймером)
	
	FTimerHandle StatsTimerHandle;  // Таймер для оновлення статистики
#pragma endregion
};
