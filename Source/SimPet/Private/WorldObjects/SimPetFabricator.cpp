// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "SimPetDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SimPetFabricatorWidget.h"
#include "WorldObjects/SimPetAnimalSubsystem.h"
#include "Components/SimPetPointsTransactionComponent.h"
#include "Widgets/SimPetHUD.h"

ASimPetFabricator::ASimPetFabricator()
{
	PointsTransactionComponent = CreateDefaultSubobject<USimPetPointsTransactionComponent>(TEXT("PointsTransactionComponent"));
}

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
	Debug::Print(__func__);

	if (!FabricatorWidgetClass)
	{
		Debug::PrintError(TEXT("FabricatorWidgetClass is not selected in Blueprint!"));
		return;
	}
	
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		USimPetFabricatorWidget *FabricatorUI = CreateWidget<USimPetFabricatorWidget>(PC, FabricatorWidgetClass);
		if (FabricatorUI)
		{
			if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(PC->GetHUD()))
				CurrentHUD->SetHUDVisibility(false);
			
			FabricatorUI->InitializeFabricator(this);
			FabricatorUI->AddToViewport();
			
			PC->FlushPressedKeys();
			PC->SetShowMouseCursor(true);
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(FabricatorUI->TakeWidget());
			PC->SetInputMode(InputMode);
		}
	}
}

void ASimPetFabricator::AttemptBuyAnimal(ESimPetAnimals AnimalType)
{
	if (!GetWorld())
		return;
	
	int32 CurrentAnimalPrice = PointsTransactionComponent->GetAnimalPrice(AnimalType);
	
	if (PointsTransactionComponent->AttemptTransaction(CurrentAnimalPrice))
	{
		USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
		if (AnimalSubsystem)
		{
			AnimalSubsystem->SpawnAnimal(AnimalType);
			
			PointsTransactionComponent->RegisterAnimalPurchase();
		}
	}
	else
	{
		Debug::Print(TEXT("Not enough points!"));
	}
}
