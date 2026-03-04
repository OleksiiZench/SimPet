// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "Characters/Animals/SimPetAnimal.h"
#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"

#include "SimPetDebugHelper.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SimPetFabricatorWidget.h"

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
	Debug::Print(__func__);
	
	if (TSubclassOf<ASimPetAnimal> *FoundClassAnimal = AnimalClassMap.Find(AnimalType))
	{
		TArray<AActor *> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetSpawnPoint::StaticClass(), FoundActors);
		
		ASimPetSpawnPoint *AnimalSpawnPoint = nullptr;
		
		// 1. Беремо перешу ліпшу ASimPetSpawnPoint
		for (AActor *Actor : FoundActors)
		{
			if (ASimPetSpawnPoint *СastAnimalSpawnPoint = Cast<ASimPetSpawnPoint>(Actor))
			{
				AnimalSpawnPoint = СastAnimalSpawnPoint;
			}
			else
				continue;
			
			// 2. Отримуємо теги точки
			FGameplayTagContainer TagContainer;
		
			if (AnimalSpawnPoint)
				AnimalSpawnPoint->GetOwnedGameplayTags(TagContainer);
		
			// 3. Точка має бути SpawnPoint.ForAnimal та не мати тварину
			if (TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForAnimal) && !TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasAnimal))
			{
				FTransform AnimalTransform = AnimalSpawnPoint->GetTransform();

				ASimPetAnimal * NewAnimal = GetWorld()->SpawnActor<ASimPetAnimal>(*FoundClassAnimal, AnimalTransform);

				if (NewAnimal)
				{
					Debug::Print(TEXT("Spawned: ") + (*FoundClassAnimal)->GetName());
			
					AnimalSpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasAnimal);
				
					break;
				}
				else
					continue;
			}
			else
				continue;
		}
	}
}
