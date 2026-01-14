// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SimPetHUD.generated.h"

class USimPetHUDWidget;
/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual  void BeginPlay() override;
	
	void SetHUDVisibility(bool bIsVisible);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USimPetHUDWidget> HUDWidgetClass;
	
private:
	UPROPERTY()
	USimPetHUDWidget *CurrentHUDWidget;
};
