// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SimPetPauseMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	FName MainMenuLevelName;
	
#pragma region UI Bindings
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_Resume;
	
	UPROPERTY(meta = (BindWidget))
	UButton *Btn_ToMainMenu;
#pragma endregion
	
#pragma region UI Callbacks
	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnToMainMenuClicked();
#pragma endregion
};
