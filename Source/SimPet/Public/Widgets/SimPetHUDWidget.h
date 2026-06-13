// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class ASimPetPlayer;
class USimPetStaminaComponent;
class ASimPetPlayerState;
class USimPetAnimalSubsystem;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
protected:
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Points;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_TotalPoints;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_OwnerAnimals;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Dog;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *DogCountText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Canary;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *CanaryCountText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Lizard;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *LizardCountText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_WildAnimals;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *Text_Animals;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock *WildAnimalCountText;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar *StaminaProgressBar;
#pragma endregion
	
#pragma region Logic
	UFUNCTION()
	void UpdateStaminaBar(float Percent);
	
	UFUNCTION()
	void UpdatePointsText(int32 Points);
	
	UFUNCTION()
	void UpdateAnimalStats();  // Функція для оновлення статистики (за таймером)
	
	FTimerHandle StatsTimerHandle;  // Таймер для оновлення статистики
#pragma endregion
	
private:
	UPROPERTY()
	USimPetStaminaComponent *CachedPlayerStaminaComponent;
	
	UPROPERTY()
	ASimPetPlayerState *CachedPlayerState;
	
	UPROPERTY()
	USimPetAnimalSubsystem *CachedAnimalSubsystem;
	
	void CacheDependencies();
	void CacheAnimalSubsystem();
	void SetupInitialValues();
	void SetupUIBindings();
	
	void BindDelegates();
	void UnbindDelegates();
};
