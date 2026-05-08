// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SimPetHUD.generated.h"

class USimPetHUDWidget;
class USimPetTimerHUDWidget;
class USimPetUISubsystem;
class USimPetNotificationWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	void SetAllHUDVisibility(bool bIsVisible);
	void SetGameplayHUDVisibility(const bool bIsVisible);
	void SetTimerHUDVisibility(const bool bIsVisible);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | UI")
	TSubclassOf<UUserWidget> GameplayHUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | UI")
	TSubclassOf<UUserWidget> TimerHUDWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet | UI")
	TSubclassOf<UUserWidget> NotificationWidgetClass;
	
private:
	UPROPERTY()
	UUserWidget *CurrentGameplayHUDWidget;
	
	UPROPERTY()
	UUserWidget *CurrentTimerHUDWidget;
	
	UPROPERTY()
	USimPetNotificationWidget *CurrentNotificationWidget;
	
	UPROPERTY()
	USimPetUISubsystem *CachedUISubsystem;
	
	void CreateWidgets();
	void CacheUISubsystem();
	void BindDelegates();
	void UnbindDelegates();
	
	UFUNCTION()
	void HandleShowNotification(bool bIsSuccess, const FString &Message);
	
	UUserWidget * CreateAndAddToViewportWidget(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder = 0);
};
