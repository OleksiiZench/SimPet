// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "Characters/SimPetBaseCharacter.h"
#include "SimPetAnimal.generated.h"

UENUM(BlueprintType)
enum class ESimPetAnimalState : uint8
{
	Happy		UMETA(DisplayName = "Happy"),
	Tired		UMETA(DisplayName = "Tired"),
	Dead		UMETA(DisplayName = "Dead")
};

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAnimal : public ASimPetBaseCharacter
{
	GENERATED_BODY()

public:
	ASimPetAnimal();

	virtual void Tick(float DeltaTime) override;
	
protected:

#pragma region State
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TimeSinceLastMeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ESimPetAnimalState AnimalState;
#pragma endregion

#pragma region Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 GameSpeed;  // За скільки секунд проходить ігрова година

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TiredThresholdHours;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeathThresholdHours;
#pragma endregion

private:
	float TimeAccumulator;
};
