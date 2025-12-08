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
	virtual void OnConstruction(const FTransform &Transform) override;

	void ToEat();
	void ToClean();
	
protected:

#pragma region State
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|State")
	bool bIsClean;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|State")
	int32 MealPerDay;  // Скільки разів за день тварина поїла

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|State")
	float TimeSinceLastMeal;  // Час з останнього харчування

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimPet|State")
	float TimeSinceLastClean;  // Час з останнього прибирання

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SimPet|State")
	ESimPetAnimalState AnimalState;
#pragma endregion

#pragma region Config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Config")
	float GameSpeed;  // За скільки секунд проходить ігрова година

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Config")
	float TiredThresholdHours;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Config")
	float DeathThresholdHours;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|Config")
	float DirtyThresholdHours;
#pragma endregion

#pragma region BodyParts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *TailMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<UStaticMeshComponent *> SpawnedEyes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<UStaticMeshComponent *> SpawnedLegs;

	// Налаштування динамічних частини тіла
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMesh * EyesAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMesh * LegsAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<FName> EyesSockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<FName> LegsSockets;
#pragma endregion

private:
	float TimeAccumulator;

	void ToDie();
	void ToDirty();
	void UpdateAnimalState();
};
