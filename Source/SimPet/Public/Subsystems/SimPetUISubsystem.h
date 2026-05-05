// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "SimPetUISubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShowUINotificationSignature, bool, bIsSuccess, const FString&, Message);

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetUISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnShowUINotificationSignature OnShowUINotification;
	
	void BroadcastNotification(bool bIsSuccess, const FString &Message);
};
