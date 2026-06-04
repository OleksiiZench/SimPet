// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetTypes/SimPetEnumTypes.h"
#include "SimPetTypes/SimPetNeedsConfig.h"

#include "SimPetNeedsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotHungrySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHungrySatisfied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotDirtySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotCleanSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNeedsDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHappyTickSignature);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetNeedsComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USimPetNeedsComponent();
	
	void InitializeNeeds(const FSimPetNeedsConfig &InConfig);
	void DisableNeedsAndBecomeHappy();
	void EnableNeeds();
	ESimPetAnimalState GetAnimalState() const;
	
	void Feed();
	void Wash();
	
	float GetTimeSinceLastMeal() const;
	float GetTimeSinceLastClean() const;
	void RestoreNeedsState(float SavedTimeSinceLastMeal, float SavedTimeSinceLastClean);
	
	UPROPERTY()
	FOnGotHungrySignature OnGotHungry;
	
	UPROPERTY()
	FOnHungrySatisfied OnHungrySatisfied;
	
	UPROPERTY()
	FOnGotDirtySignature OnGotDirty;
	
	UPROPERTY()
	FOnGotCleanSignature OnGotClean;
	
	UPROPERTY()
	FOnNeedsDepletedSignature OnNeedsDepleted;
	
	UPROPERTY()
	FOnHappyTickSignature OnHappyTick;
	
private:
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
	
	FSimPetNeedsConfig CurrentConfig;
	
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
