// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SimPetPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void SetInputMode_Game();
	void SetInputMode_UI(UUserWidget *FocusWidget = nullptr);
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultMappingContext;
};
