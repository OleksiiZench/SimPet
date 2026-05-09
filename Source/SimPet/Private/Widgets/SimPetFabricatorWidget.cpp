// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Widgets/SimPetFabricatorWidget.h"

#include "Components/TextBlock.h"
#include "Input/Reply.h"

#include "WorldObjects/SimPetFabricator.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "Components/SimPetPointsTransactionComponent.h"
#include "SimPetEnumTypes.h"
#include "Widgets/SimPetHUD.h"
#include "Core/SimPetPlayerState.h"

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
	
	CachePlayerState();
	BindToPointsUpdate();
	
	if (CachedPlayerState)
	{
		int32 CurrentPoints = CachedPlayerState->GetCurrentPoints();
		UpdatePointsText(CurrentPoints);
	}
}

void USimPetFabricatorWidget::NativeDestruct()
{
	UnbindToPointsUpdate();
	
	Super::NativeDestruct();
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
			CurrentHUD->SetGameplayHUDVisibility(true);
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

FReply USimPetFabricatorWidget::NativeOnMouseButtonDown(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	return FReply::Handled().SetUserFocus(GetCachedWidget().ToSharedRef(), EFocusCause::Mouse);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetFabricatorWidget::UpdatePointsText(const int32 Points)
{
	const FString FormattedString = FString::Printf(TEXT("Points: %d"), Points);
	if (PointsText)
	{
		PointsText->SetText(FText::FromString(FormattedString));
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetFabricatorWidget::UpdateAnimalPricesUI()
{
	if (!CachedTransactionComponent.IsValid())
		return;

	if (PriceDogText)
	{
		const int32 PriceDog = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Dog);

		const FString FormattedString = FString::Printf(TEXT("%d"), PriceDog);
		PriceDogText->SetText(FText::FromString(FormattedString));
	}
	
	if (PriceCanaryText)
	{
		const int32 PriceCanary = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Canary);
        
        const FString FormattedString = FString::Printf(TEXT("%d"), PriceCanary);
        PriceCanaryText->SetText(FText::FromString(FormattedString));
	}
	
	if (PriceLizardText)
	{
		const int32 PriceLizard = CachedTransactionComponent->GetAnimalPrice(ESimPetAnimals::EA_Lizard);
        
		const FString FormattedString = FString::Printf(TEXT("%d"), PriceLizard);
		PriceLizardText->SetText(FText::FromString(FormattedString));
	}
}

void USimPetFabricatorWidget::CachePlayerState()
{
	CachedPlayerState = Cast<ASimPetPlayerState>(GetOwningPlayerState());
	if (CachedPlayerState == nullptr)
	{
		Debug::PrintError(TEXT("Caching PlayerState failed!"));
	}
}

void USimPetFabricatorWidget::BindToPointsUpdate()
{	
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.AddDynamic(this, &ThisClass::UpdatePointsText);
}

void USimPetFabricatorWidget::UnbindToPointsUpdate()
{
	if (CachedPlayerState)
		CachedPlayerState->OnPointsChanged.RemoveDynamic(this, &ThisClass::UpdatePointsText);
}
