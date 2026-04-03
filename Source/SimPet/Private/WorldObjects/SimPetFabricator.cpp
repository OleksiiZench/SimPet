// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SimPetFabricatorWidget.h"
#include "WorldObjects/SimPetAnimalSubsystem.h"

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
	Debug::Print(__func__);

	if (!FabricatorWidgetClass)
	{
		Debug::Print(TEXT("Error: FabricatorWidgetClass is not selected in Blueprint!"));
		return;
	}
	
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		USimPetFabricatorWidget *FabricatorUI = CreateWidget<USimPetFabricatorWidget>(PC, FabricatorWidgetClass);
		if (FabricatorUI)
		{
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

void ASimPetFabricator::RequestSpawnAnimal(ESimPetAnimals AnimalType)
{
	if (!GetWorld())
		return;
	
	if (TSubclassOf<ASimPetAnimal> *FoundClassAnimal = AnimalClassMap.Find(AnimalType))
	{
		USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
		
		if (!AnimalSubsystem)
			return;
		
		AnimalSubsystem->SpawnAnimal(*FoundClassAnimal);
	}
}
