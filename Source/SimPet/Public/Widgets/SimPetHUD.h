// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SimPetHUD.generated.h"

class USimPetHUDWidget;
class USimPetTimerHUDWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual  void BeginPlay() override;
	
	void SetAllHUDVisibility(bool bIsVisible);
	void SetGameplayHUDVisibility(bool bIsVisible);
	void SetTimerHUDVisibility(bool bIsVisible);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | UI")
	TSubclassOf<UUserWidget> GameplayHUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | UI")
	TSubclassOf<UUserWidget> TimerHUDWidgetClass;
	
private:
	UPROPERTY()
	UUserWidget *CurrentGameplayHUDWidget;
	
	UPROPERTY()
	UUserWidget *CurrentTimerHUDWidget;
	
	UUserWidget * CreateAndAddToViewportWidget(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
};
