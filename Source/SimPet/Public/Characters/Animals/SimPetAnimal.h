// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "Characters/SimPetBaseCharacter.h"
#include "SimPetEnumTypes.h"

#include "SimPetAnimal.generated.h"

class UBehaviorTree;
class USimPetPointsTransactionComponent;
class UWidgetComponent;
class USimPetAnimalStatusWidget;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAnimal : public ASimPetBaseCharacter
{
	GENERATED_BODY()

public:
	ASimPetAnimal();

	virtual void OnConstruction(const FTransform &Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Feed();
	void Wash();
	
	ESimPetAnimalState GetAnimalState();
	
#pragma region AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree *BehaviorTreeAsset;
	
	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bUseCustomHomeLocation;
	
	UPROPERTY(EditInstanceOnly, Category = "AI",  meta = (EditCondition = "bUseCustomHomeLocation"))
	FVector CustomHomeLocation;
#pragma endregion
	
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

	/* Налаштування динамічних частини тіла */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<UStaticMeshComponent *> SpawnedEyes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<UStaticMeshComponent *> SpawnedLegs;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts", meta = (ClampMin = "0", ClampMax = "4", UIMin = "0", UIMax = "4"))
	int32 EyesCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts", meta = (ClampMin = "0", ClampMax = "4", UIMin = "0", UIMax = "4"))
	int32 LegsCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMesh * EyesAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMesh * LegsAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<FName> EyesSockets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	TArray<FName> LegsSockets;
	
	float LegLiftAngle = 45.0f;
	
	virtual void AnimateLegs(float DeltaTime, float CurrentTime);
#pragma endregion
	
	UCharacterMovementComponent *MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimPet|UI")
	UWidgetComponent *StatusWidhetComponent;
	
private:
	FTimerHandle MetabolismTimerHandle;
	
	USimPetPointsTransactionComponent *PointsTransactionComponent;
	USimPetAnimalStatusWidget *AnimalStatusWidget;
	
	void Die();
	void BecomeDirty();
	
	void SetupBody();
	void SetupComponent();
	void CacheAnimalStatusWidget();
	
	void OnMetabolismTick();
	void CheckPhysicalAnimalState();
	void UpdateAnimalState();
	void GeneratePointsIfAnimalIsHappy();
	void StartMetabolismTimer();
	void StopMetabolismTimer();
};
