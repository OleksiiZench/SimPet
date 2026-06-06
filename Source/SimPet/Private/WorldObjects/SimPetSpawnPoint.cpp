// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "WorldObjects/SimPetSpawnPoint.h"

#include "SimPetGameplayTags.h"
#include "Items/SimPetAnimalFeed.h"
#include "Save/SimPetSaveGame.h"
#include "Subsystems/SimPetAnimalSubsystem.h"
#include "Subsystems/SimPetItemSubsystem.h"
#include "Subsystems/SimPetSaveSubsystem.h"

void ASimPetSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
	RegisterSpawnPointInAnimalSubsystem();
	RegisterSpawnPointInSaveSubsystem();
}

void ASimPetSpawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveSpawnPointFromSaveSubsystem();
	
	Super::EndPlay(EndPlayReason);
}

void ASimPetSpawnPoint::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ConfigTags);
	TagContainer.AppendTags(DynamicTags);
}

void ASimPetSpawnPoint::SaveActorData_Implementation(USimPetSaveGame *SaveObject)
{
	if (SaveObject)
		SaveObject->SpawnPointsTagsSaveData.Add(GetFName(), DynamicTags);
}

void ASimPetSpawnPoint::LoadActorData_Implementation(USimPetSaveGame *SaveObject)
{
	if (!SaveObject || !SaveObject->SpawnPointsTagsSaveData.Contains(GetFName()))
		return;
	
	DynamicTags = SaveObject->SpawnPointsTagsSaveData[GetFName()];
	
	if (DynamicTags.HasTag(SimPetGameplayTags::Spawn_Point_HasFeed))
	{
		DynamicTags.RemoveTag(SimPetGameplayTags::Spawn_Point_HasFeed);
		
		USimPetItemSubsystem *ItemSubsystem = GetWorld()->GetSubsystem<USimPetItemSubsystem>();
		if (ItemSubsystem)
			ItemSubsystem->SpawnFeedOnSinglePoint(this);
	}
}

void ASimPetSpawnPoint::AddGameplayTags(const FGameplayTag& Tag)
{
	if (Tag.IsValid())
		DynamicTags.AddTag(Tag);
}

void ASimPetSpawnPoint::RemoveGameplayTags(const FGameplayTag &Tag)
{
	if (Tag.IsValid())
		DynamicTags.RemoveTag(Tag);
}

void ASimPetSpawnPoint::HandleFeedPickedUp(ASimPetAnimalFeed *PickedFeed)
{
	ClearFeedTag();
	
	if (PickedFeed)
		PickedFeed->OnFeedPickedUp.RemoveDynamic(this, &ASimPetSpawnPoint::HandleFeedPickedUp);
}

void ASimPetSpawnPoint::HandleFeedDestroyed(AActor *DestroyedActor)
{
	ClearFeedTag();
	
	if (DestroyedActor)
	{
		DestroyedActor->OnDestroyed.RemoveDynamic(this, &ASimPetSpawnPoint::HandleFeedDestroyed);
	}
}

void ASimPetSpawnPoint::RegisterSpawnPointInAnimalSubsystem()
{
	USimPetAnimalSubsystem *AnimalSubsystem = GetWorld()->GetSubsystem<USimPetAnimalSubsystem>();
	if (AnimalSubsystem)
		AnimalSubsystem->RegisterSpawnPoint(this);
}

void ASimPetSpawnPoint::RegisterSpawnPointInSaveSubsystem()
{
	USimPetSaveSubsystem *SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	if (SaveSubsystem)
		SaveSubsystem->RegisterSavableActor(this);
}

void ASimPetSpawnPoint::RemoveSpawnPointFromSaveSubsystem()
{
	USimPetSaveSubsystem *SaveSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USimPetSaveSubsystem>();
	if (SaveSubsystem)
		SaveSubsystem->UnregisterSavableActor(this);
}

void ASimPetSpawnPoint::ClearFeedTag()
{
	DynamicTags.RemoveTag(SimPetGameplayTags::Spawn_Point_HasFeed);
}
