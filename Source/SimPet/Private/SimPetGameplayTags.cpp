// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "SimPetGameplayTags.h"

namespace SimPetGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "InputTag.Interact");

	/** Interactable Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Interactable_Food, "Interactable.Food");
	UE_DEFINE_GAMEPLAY_TAG(Interactable_Waste, "Interactable.Waste");

	/** Spawn Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Spawn_Point, "Spawn.Point");
	UE_DEFINE_GAMEPLAY_TAG(Spawn_Point_HasAnimal, "Spawn.Point.HasAnimal");
}
