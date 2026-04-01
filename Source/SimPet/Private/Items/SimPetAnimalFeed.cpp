// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Items/SimPetAnimalFeed.h"

#include "Characters/Animals/SimPetAnimal.h"
#include "Interfaces/SimPetItemContainer.h"

void ASimPetAnimalFeed::Interact_Implementation(AActor *InstigatorActor)
{
	Super::Interact_Implementation(InstigatorActor);
	
	OnFeedPickedUp.Broadcast(this);
}

bool ASimPetAnimalFeed::TryInteractWithAnotherActor(AActor *TargetActor)
{
	if (ASimPetAnimal *Animal = Cast<ASimPetAnimal>(TargetActor))
	{
		Animal->FeedAnimal();
		
		Destroy();
		
		return true;
	}
	
	if (TargetActor->Implements<USimPetItemContainer>())
	{
		return ISimPetItemContainer::Execute_TryAddItemToContainer(TargetActor, this);
	}
	
	return false;
}

bool ASimPetAnimalFeed::TryFeedAnimal(AActor *TargetActor)
{
	if (ASimPetAnimal *Animal = Cast<ASimPetAnimal>(TargetActor))
	{
		Animal->FeedAnimal();
		
		Destroy();
		
		return true;
	}
	
	return false;
}
