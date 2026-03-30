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
	
	virtual void BeginPlay() override;
	
	ESimPetAnimalState GetAnimalState();
	
	void Feed();
	void Wash();
	
	UPROPERTY()
	FOnGotHungrySignature OnGotHungry;
	
	UPROPERTY()
	FOnGotDirtySignature OnGotDirty;
	
	UPROPERTY()
	FOnDiedSignature OnDied;
	
	UPROPERTY()
	FOnHappyTickSignature OnHappyTick;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float SecondsPerGameHour;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float HungryThresholdHours;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float DeathThresholdHours;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Config")
	float DirtyThresholdHours;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	bool bNeedsCleaning;
	
	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	bool bNeedsFeed;

	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	float TimeSinceLastMeal;  // Час з останнього харчування

	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	float TimeSinceLastClean;  // Час з останнього прибирання

	UPROPERTY(VisibleAnywhere, Category = "SimPet|State")
	ESimPetAnimalState AnimalState;
	
	FTimerHandle NeedsTimerHandle;
	
	void StartNeedsTimer();
	void StopNeedsTimer();
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
