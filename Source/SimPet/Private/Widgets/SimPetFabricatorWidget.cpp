// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetFabricatorWidget.h"

#include "SimPetDebugHelper.h"

void USimPetFabricatorWidget::InitializeFabricator(ASimPetFabricator* InFabricator)
{
	LinkedFabricator = InFabricator;
}

void USimPetFabricatorWidget::OnAnimalSelected(ESimPetAnimals AnimalType)
{
	if (LinkedFabricator.IsValid())
	{
		LinkedFabricator->RequestSpawnAnimal(AnimalType);
		
		RemoveFromParent();
		
		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->SetShowMouseCursor(false);
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}
