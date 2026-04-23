// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetFabricatorWidget.h"

#include "Components/TextBlock.h"
#include "Input/Reply.h"

#include "WorldObjects/SimPetFabricator.h"
#include "WorldObjects/SimPetAnimalSubsystem.h"
#include "Components/SimPetPointsTransactionComponent.h"
#include "SimPetEnumTypes.h"
#include "Widgets/SimPetHUD.h"

#include "SimPetDebugHelper.h"

USimPetFabricatorWidget::USimPetFabricatorWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void USimPetFabricatorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UWorld *World = GetWorld())
	{
		AnimalSubsystem = World->GetSubsystem<USimPetAnimalSubsystem>();
	}
}

FReply USimPetFabricatorWidget::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	
	bool bIsCloseKey = (PressedKey == EKeys::Escape) ||
					   (PressedKey == EKeys::Gamepad_FaceButton_Right);
	
	if (bIsCloseKey)
	{
		OnCloseMenu();
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USimPetFabricatorWidget::InitializeFabricator(ASimPetFabricator* InFabricator)
{
	LinkedFabricator = InFabricator;
	
	if (LinkedFabricator.IsValid())
	{
		CachedTransactionComponent = LinkedFabricator->FindComponentByClass<USimPetPointsTransactionComponent>();
		
		UpdateAnimalPricesUI();
	}
}

void USimPetFabricatorWidget::OnBuyAnimalClicked(ESimPetAnimals AnimalType)
{
	if (LinkedFabricator.IsValid())
	{
		if (LinkedFabricator->AttemptBuyAnimal(AnimalType))
		{
			UpdateAnimalPricesUI();
		}
	}
}

void USimPetFabricatorWidget::OnCloseMenu()
{
	RemoveFromParent();
		
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
		
		if (ASimPetHUD *CurrentHUD = Cast<ASimPetHUD>(PC->GetHUD()))
			CurrentHUD->SetHUDVisibility(true);
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

void USimPetFabricatorWidget::UpdateAnimalPricesUI()
{
	if (!CachedTransactionComponent.IsValid())
		return;

	if (PriceDogText)
	{
		int32 PriceDog = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Dog);

		FString FormattedString = FString::Printf(TEXT("%d"), PriceDog);
		PriceDogText->SetText(FText::FromString(FormattedString));
	}
	
	if (PriceCanaryText)
	{
		int32 PriceCanary = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Canary);
        
        FString FormattedString = FString::Printf(TEXT("%d"), PriceCanary);
        PriceCanaryText->SetText(FText::FromString(FormattedString));
	}
	
	if (PriceLizardText)
	{
		int32 PriceLizard = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Lizard);
        
		FString FormattedString = FString::Printf(TEXT("%d"), PriceLizard);
		PriceLizardText->SetText(FText::FromString(FormattedString));
	}
}
