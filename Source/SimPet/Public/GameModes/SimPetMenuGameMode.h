// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameModes/SimPetBaseGameMode.h"

#include "SimPetMenuGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetMenuGameMode : public ASimPetBaseGameMode
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
};
