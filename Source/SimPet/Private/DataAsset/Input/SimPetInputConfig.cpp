// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "DataAsset/Input/SimPetInputConfig.h"

const UInputAction *USimPetInputConfig::FindNativeInputActionByTag(const FGameplayTag &InInputTag) const
{
	for (const FSimPetInputActionConfig &InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputAction && InputActionConfig.InputTag == InInputTag)
			return InputActionConfig.InputAction;
	}

	return nullptr;
}
