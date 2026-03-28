// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetNeedsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotHungrySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotDirtySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDiedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHappyTickSignature);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetNeedsComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USimPetNeedsComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "SimPet|Needs|Events")
	FOnGotHungrySignature OnGotHungry;
	
	UPROPERTY(BlueprintAssignable, Category = "SimPet|Needs|Events")
	FOnGotDirtySignature OnGotDirty;
	
	UPROPERTY(BlueprintAssignable, Category = "SimPet|Needs|Events")
	FOnDiedSignature OnDied;
	
	UPROPERTY(BlueprintAssignable, Category = "SimPet|Needs|Events")
	FOnHappyTickSignature OnHappyTick;
	
	void StartNeedsTimer();
	void StopNeedsTimer();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float GameSpeed;  // За скільки секунд проходить ігрова година
	
	FTimerHandle NeedsTimerHandle;
	
	void OnNeedsTick();
};
