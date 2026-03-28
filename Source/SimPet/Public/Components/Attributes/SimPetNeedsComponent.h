// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetEnumTypes.h"

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
	
	ESimPetAnimalState GetAnimalState();
	
	void StartNeedsTimer();
	void StopNeedsTimer();
	
	void Feed();
	void Wash();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float SecondsPerGameHour;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	bool bNeedsCleaning;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	bool bNeedsFeed;

	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	float TimeSinceLastMeal;  // Час з останнього харчування

	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	float TimeSinceLastClean;  // Час з останнього прибирання

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|State")
	ESimPetAnimalState AnimalState;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float HungryThresholdHours;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float DeathThresholdHours;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float DirtyThresholdHours;
	
	FTimerHandle NeedsTimerHandle;
	
	void OnNeedsTick();
	
	void CheckPhysicalCharacterState();
	
	void Die();
	void BecomeDirty();
	void BecomeHungry();
	
	void TryEnterHappyState();
	void EnterHappyState();
	void EnterTiredState();
	void EnterDeadState();
};
