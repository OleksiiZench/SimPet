// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"

#include "SimPetCameraComponent.generated.h"

class ASimPetPlayer;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class SIMPET_API USimPetCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	USimPetCameraComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
private:
	UPROPERTY()
	ASimPetPlayer *Player;
	
	UPROPERTY()
	UCharacterMovementComponent *CharacterMovementComp;
	
	float BobVerticalAmplitude;  // Наскільки сильно коливає вгору/ вниз
	float BobHorizontalAmplitude;  // Наскільки сильно коливає вліво/ вправо
	float BobFrequencyMultiplier;  // Множник швидкості коливання
	float BobTimer;  // Timer for a sine wave
	FVector DefaultCameraRelativeLocation;  // Координати камери в спокої
	
	void UpdateCameraBob(float DeltaTime);
	void CachePlayerAndCharacterMovementComp();
};
