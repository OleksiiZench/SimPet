// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace SimPetGameplayTags
{
	/** Input Tags **/
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Pause);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Sprint);

	/** Interactable Tags **/
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Interactable_Food);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Interactable_Waste);
	
	/** Spawn Tags **/
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spawn_Point);
	SIMPET_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spawn_Point_HasAnimal);
}
