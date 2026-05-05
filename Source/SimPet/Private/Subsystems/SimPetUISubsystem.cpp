// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "Subsystems/SimPetUISubsystem.h"

// ReSharper disable once CppMemberFunctionMayBeConst
void USimPetUISubsystem::BroadcastNotification(bool bIsSuccess, const FString &Message)
{
	OnShowUINotification.Broadcast(bIsSuccess, Message);
}
