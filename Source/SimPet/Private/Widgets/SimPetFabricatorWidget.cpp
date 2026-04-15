// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetFabricatorWidget.h"

#include "WorldObjects/SimPetFabricator.h"
#include "WorldObjects/SimPetAnimalSubsystem.h"

#include "SimPetDebugHelper.h"

void USimPetFabricatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UWorld *World = GetWorld())
	{
		AnimalSubsystem = World->GetSubsystem<USimPetAnimalSubsystem>();
	}
}

void USimPetFabricatorWidget::InitializeFabricator(ASimPetFabricator* InFabricator)
{
	LinkedFabricator = InFabricator;
}

void USimPetFabricatorWidget::OnBuyAnimalClicked(ESimPetAnimals AnimalType)
{
	if (LinkedFabricator.IsValid())
	{
		LinkedFabricator->AttemptBuyAnimal(AnimalType);
		
		OnCloseMenu();
	}
}

void USimPetFabricatorWidget::OnCloseMenu()
{
	RemoveFromParent();
		
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void USimPetFabricatorWidget::MoveAnimalToForest()
{
	if (AnimalSubsystem == nullptr)
	{
		return;
	}
	
	int32 NumberAnimalInOwner = AnimalSubsystem->GetTotalNumberOwnerAnimals();
	if (NumberAnimalInOwner <= 0)
	{
		Debug::Print(TEXT("The owner does not have any animals!"));
		return;
	}

	AnimalSubsystem->MoveAnimalToForest();
}

void USimPetFabricatorWidget::MoveAnimalToOwner()
{
	if (AnimalSubsystem == nullptr)
	{
		return;
	}
	
	int32 NumberAnimalInForest = AnimalSubsystem->GetTotalNumberWildAnimals();
	if (NumberAnimalInForest <= 0)
	{
		Debug::Print(TEXT("There are no animals in the forest!"));
		return;
	}

	AnimalSubsystem->MoveAnimalToOwner();
}
