// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetFabricator.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/SimPetFabricatorWidget.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "Components/SimPetPointsTransactionComponent.h"
#include "Widgets/SimPetHUD.h"

#include "SimPetDebugHelper.h"
#include "Subsystems/SimPetUISubsystem.h"

#define LOCTEXT_NAMESPACE "Fabricator"

const FText PurchaseSuccessMsg = LOCTEXT("PurchaseSuccess", "Successful purchase of an animal!");
const FText NoPointsMsg = LOCTEXT("NoPoints", "Failed animal purchase. Not enough points!");
const FText SpawnErrorMsg = LOCTEXT("SpawnError", "Failed animal purchase. Failed spawn!");
const FText MoveToForestMsg = LOCTEXT("MoveToForest", "Successful move to Forest!");
const FText MoveToOwnerMsg = LOCTEXT("MoveToOwner", "Successful move to Owner!");
const FText UnsuccessfulMoveMsg = LOCTEXT("UnsuccessfulMoveMsg", "Unsuccessful move!");

ASimPetFabricator::ASimPetFabricator()
{
	PointsTransactionComponent = CreateDefaultSubobject<USimPetPointsTransactionComponent>(TEXT("PointsTransactionComponent"));
}

void ASimPetFabricator::BeginPlay()
{
	Super::BeginPlay();
	
	CacheUISubsystem();
	CacheAnimalSubsystem();
}

void ASimPetFabricator::Interact_Implementation(AActor *InstigatorActor)
{
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
		BroadcastResultNotification(false, NoPointsMsg.ToString());
		
		return false;
	}

	if (!CachedAnimalSubsystem || !CachedAnimalSubsystem->SpawnAnimal(AnimalType))
	{
		BroadcastResultNotification(false, SpawnErrorMsg.ToString());
		
		return false;
	}

	PointsTransactionComponent->ConsumePoints(CurrentAnimalPrice);
	PointsTransactionComponent->RegisterAnimalPurchase();

	BroadcastResultNotification(true, PurchaseSuccessMsg.ToString());
	
	return true;
}

void ASimPetFabricator::MoveAnimalToForest()
{
	if (CachedAnimalSubsystem == nullptr)
	{
		return;
	}
	
	int32 NumberAnimalInOwner = CachedAnimalSubsystem->GetTotalNumberOwnerAnimals();
	if (NumberAnimalInOwner <= 0)
	{
		BroadcastResultNotification(false, UnsuccessfulMoveMsg.ToString());
		
		return;
	}

	CachedAnimalSubsystem->MoveAnimalToForest();
	
	BroadcastResultNotification(true, MoveToForestMsg.ToString());
}

void ASimPetFabricator::MoveAnimalToOwner()
{
	if (CachedAnimalSubsystem == nullptr)
	{
		return;
	}
	
	int32 NumberAnimalInForest = CachedAnimalSubsystem->GetTotalNumberWildAnimals();
	if (NumberAnimalInForest <= 0)
	{
		BroadcastResultNotification(false, UnsuccessfulMoveMsg.ToString());
		
		return;
	}

	CachedAnimalSubsystem->MoveAnimalToOwner();
	
	BroadcastResultNotification(true, MoveToOwnerMsg.ToString());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASimPetFabricator::BroadcastResultNotification(bool bIsSuccess, const FString &Message)
{
	if (CachedUISubsystem)
	{
		CachedUISubsystem->BroadcastNotification(bIsSuccess, Message);
	}
}

void ASimPetFabricator::CacheAnimalSubsystem()
{
	if (GetWorld())
	{
		CachedAnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	}
}

void ASimPetFabricator::CacheUISubsystem()
{
	if (GetWorld())
	{
		CachedUISubsystem = GetWorld()->GetSubsystem<USimPetUISubsystem>();
	}
}

#undef LOCTEXT_NAMESPACE
