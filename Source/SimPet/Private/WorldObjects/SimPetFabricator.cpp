// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "SimPetDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SimPetFabricatorWidget.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
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
				CurrentHUD->SetGameplayHUDVisibility(false);
			
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

bool ASimPetFabricator::AttemptBuyAnimal(ESimPetAnimals AnimalType)
{
	if (!GetWorld() || !PointsTransactionComponent)
		return false;
	
	int32 CurrentAnimalPrice = PointsTransactionComponent->GetAnimalPrice(AnimalType);
	if (!PointsTransactionComponent->CanAfford(CurrentAnimalPrice))
	{
		Debug::Print(TEXT("Not enough points!"));
		return false;
	}
	
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();

	if (!AnimalSubsystem || !AnimalSubsystem->SpawnAnimal(AnimalType))
	{
		Debug::PrintError(TEXT("Cannot spawn animal!"));
		return false;
	}

	PointsTransactionComponent->ConsumePoints(CurrentAnimalPrice);
	PointsTransactionComponent->RegisterAnimalPurchase();

	return true;
}
