// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetItemSubsystem.h"

#include "Kismet/GameplayStatics.h"

#include "Core/Settings/SimPetItemSettings.h"
#include "Items/SimPetAnimalFeed.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "WorldObjects/SimPetSpawnPoint.h"
#include "SimPetGameplayTags.h"
#include "Interfaces/SimPetItemContainer.h"
#include "Save/Structures/SimPetItemSaveData.h"

void USimPetItemSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	Super::Initialize(Collection);
	
	CacheSpawnFeedClassFromItemSettings();
}

void USimPetItemSubsystem::SpawnFeedOnRelevantPoints()
{
	if (CachedSpawnFeedClass == nullptr)
		return;
	
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem == nullptr)
		return;
	
	const TArray<ASimPetSpawnPoint *> &AllSpawnPoints = AnimalSubsystem->GetAllSpawnPoints();

	for (ASimPetSpawnPoint *SpawnPoint : AllSpawnPoints)
	{
		if (!SpawnPoint)
			continue;
			
		FGameplayTagContainer TagContainer;
		SpawnPoint->GetOwnedGameplayTags(TagContainer);
			
		if (!TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_ForFeed) || 
			 TagContainer.HasTag(SimPetGameplayTags::Spawn_Point_HasFeed))
		{
			continue;
		}
		
		FTransform FeedTransform = SpawnPoint->GetTransform();
		ASimPetAnimalFeed *NewFeed = GetWorld()->SpawnActor<ASimPetAnimalFeed>(CachedSpawnFeedClass, FeedTransform);
		if (NewFeed)
		{
			NewFeed->SetItemState(ESimPetItemState::ManagedBySpawnPoint);
			
			SpawnPoint->AddGameplayTags(SimPetGameplayTags::Spawn_Point_HasFeed);
			
			NewFeed->OnFeedPickedUp.AddDynamic(SpawnPoint, &ASimPetSpawnPoint::HandleFeedPickedUp);
			NewFeed->OnDestroyed.AddDynamic(SpawnPoint, &ASimPetSpawnPoint::HandleFeedDestroyed);
		}
	}
}

ASimPetItem *USimPetItemSubsystem::RestoreItemFromSaveData(const FSimPetItemSaveData &ItemSaveData, const FTransform &SpawnTransform)
{
	if (!IsValid(ItemSaveData.ItemClass))
		return nullptr;
	
	ASimPetItem *MainItem = SpawnItem(ItemSaveData.ItemClass, SpawnTransform);
	if (MainItem == nullptr)
		return nullptr;
	
	RestoreItemPayload(MainItem, ItemSaveData.PayloadClasses, SpawnTransform);
	
	return MainItem;
}

TArray<FSimPetDroppedItemSaveData> USimPetItemSubsystem::GetDroppedItemsSaveData() const
{
	TArray<FSimPetDroppedItemSaveData> ResultArray;
	
	TArray<AActor *> AllItems;
	// TODO: Варто позбутися GetAllActorsOfClass і реалізувати це через патерн Реєстратор
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetItem::StaticClass(), AllItems);
	
	for (AActor *Actor : AllItems)
	{
		ASimPetItem *Item = Cast<ASimPetItem>(Actor);
		
		if (!IsValid(Item) || Item->GetItemState() != ESimPetItemState::Dropped)
			continue;
		
		FSimPetDroppedItemSaveData DroppedItemData;
		DroppedItemData.Transform = Item->GetActorTransform();
		DroppedItemData.ItemData.ItemClass = Item->GetClass();
		
		if (Item->Implements<USimPetItemContainer>())
		{
			ISimPetItemContainer::Execute_GetContainerPayloadClasses(Item, DroppedItemData.ItemData.PayloadClasses);
		}
		
		ResultArray.Add(DroppedItemData);
	}
	
	return ResultArray;
}

void USimPetItemSubsystem::RestoreDroppedItemsFromSaveData(const TArray<FSimPetDroppedItemSaveData> &SavedDroppedItems)
{
	// 1. Очищення існуючих предметів на рівні
	// TODO: Обміркувати доцільність знищення всіх предметів на карті
	TArray<AActor *> AllItems;
	// TODO: Варто позбутися GetAllActorsOfClass і реалізувати це через патерн Реєстратор
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASimPetItem::StaticClass(), AllItems);
	
	for (AActor *Actor : AllItems)
	{
		ASimPetItem *Item = Cast<ASimPetItem>(Actor);
		if (IsValid(Item) && Item->GetItemState() == ESimPetItemState::Dropped)
			Item->Destroy();
	}
	
	// 2. Спавн предметів зі збереження
	for (const FSimPetDroppedItemSaveData &DroppedItemData : SavedDroppedItems)
	{
		RestoreItemFromSaveData(DroppedItemData.ItemData, DroppedItemData.Transform);
	}
}

void USimPetItemSubsystem::RestoreItemPayload(ASimPetItem *ContainerItem, const TArray<TSubclassOf<ASimPetItem>> &PayloadClasses, const FTransform &SpawnTransform)
{
	if (!IsValid(ContainerItem) || PayloadClasses.IsEmpty() || !ContainerItem->Implements<USimPetItemContainer>())
		return;

	for (TSubclassOf<ASimPetItem> PayloadItemClass : PayloadClasses)
	{
		if (!IsValid(PayloadItemClass))
			continue;
		
		ASimPetItem *SpawnedPayloadItem = SpawnItem(PayloadItemClass, SpawnTransform);
		if (SpawnedPayloadItem)
		{
			const bool bAdded= ISimPetItemContainer::Execute_TryAddItemToContainer(ContainerItem, SpawnedPayloadItem);
			if (!bAdded)
				SpawnedPayloadItem->Destroy();
		}
	}
	
}

// ReSharper disable once CppMemberFunctionMayBeConst
ASimPetItem *USimPetItemSubsystem::SpawnItem(TSubclassOf<ASimPetItem> ItemClass, FTransform Transform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (ItemClass)
	{
		return GetWorld()->SpawnActor<ASimPetItem>(ItemClass, Transform, SpawnParams);
	}
	
	return nullptr;
}

void USimPetItemSubsystem::CacheSpawnFeedClassFromItemSettings()
{
	const USimPetItemSettings *Settings = GetDefault<USimPetItemSettings>();
	if (Settings)
	{
		CachedSpawnFeedClass = Settings->SpawnFeedClass;
	}
}
