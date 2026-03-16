// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "SimPetStaminaComponent.generated.h"

class ASimPetPlayer;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentValue, float, Precent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPET_API USimPetStaminaComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USimPetStaminaComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	void SetSprint(bool bIsSprint);
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaChanged OnStaminaChanged;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MaxStamina;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaDrainRate;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaRegenRate;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float CurrentStamina;
	
	bool bIsMoving;
	bool bIsSprinting = false;
	
	ASimPetPlayer *Player;
	UCharacterMovementComponent *CharacterMovementComp;
	
	void UpdateStamina(float DeltaTime);
	void CachePlayerAndCharacterMovementComp();
	bool IsMovingHorizontally();
};
