// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "Characters/SimPetBaseCharacter.h"
#include "SimPetEnumTypes.h"

#include "Interfaces/SimPetInteractable.h"

#include "SimPetAnimal.generated.h"

class UBehaviorTree;
class USimPetPointsTransactionComponent;
class UWidgetComponent;
class USimPetAnimalNeedsStatusWidget;
class USimPetNeedsComponent;
class USimPetAutoHidingWidgetComponent;
class ASimPetAnimalWaste;

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetAnimal : public ASimPetBaseCharacter, public ISimPetInteractable
{
	GENERATED_BODY()

public:
	ASimPetAnimal();

	virtual void OnConstruction(const FTransform &Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//~ Begin ISimPetInteractable Interface
	virtual void Interact_Implementation(AActor *InstigatorActor) override;
	//~ End ISimPetInteractable Interface
	
	void FeedAnimal();
	
	ESimPetAnimalState GetAnimalState();
	
#pragma region AI
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|AI")
	UBehaviorTree *BehaviorTreeAsset;
	
	UPROPERTY(EditInstanceOnly, Category = "SimPet|AI")
	bool bUseCustomHomeLocation;
	
	UPROPERTY(EditInstanceOnly, Category = "SimPet|AI",  meta = (EditCondition = "bUseCustomHomeLocation"))
	FVector CustomHomeLocation;
#pragma endregion
	
protected:
#pragma region BodyParts
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *HeadMesh;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *TailMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts", meta = (ClampMin = "0", ClampMax = "4", UIMin = "0", UIMax = "4"))
	int32 EyesCount;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts", meta = (ClampMin = "0", ClampMax = "4", UIMin = "0", UIMax = "4"))
	int32 LegsCount;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	UStaticMesh * EyesAsset;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	UStaticMesh * LegsAsset;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	TArray<FName> EyesSockets;

	UPROPERTY(EditDefaultsOnly, Category = "SimPet|BodyParts")
	TArray<FName> LegsSockets;
	
	UPROPERTY()
	TArray<UStaticMeshComponent *> SpawnedEyes;

	UPROPERTY()
	TArray<UStaticMeshComponent *> SpawnedLegs;
	
	float LegLiftAngle;
	
	virtual void AnimateLegs(float DeltaTime, float CurrentTime);
	
	void RebuildBodyParts();
#pragma endregion
	
	UPROPERTY(EditDefaultsOnly, Category = "SimPet|Needs")
	TSubclassOf<ASimPetAnimalWaste> WasteClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|UI")
	USimPetAutoHidingWidgetComponent *StatusWidgetComponent;
	
	UPROPERTY()
	UCharacterMovementComponent *MovementComponent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USimPetNeedsComponent *NeedsComponent;
	
	UPROPERTY()
	USimPetPointsTransactionComponent *PointsTransactionComponent;
	
	USimPetAnimalNeedsStatusWidget *AnimalStatusWidget;
	
	UFUNCTION()
	void HandleHappyTick();
	
	UFUNCTION()
	void HandleDied();
	
	UFUNCTION()
	void HandleGotDirty();
	
	UFUNCTION()
	void HandleGotHungry();
	
	UFUNCTION()
	void HandleWasteCleaned();
	
	void InitializeBaseBody();
	void SetupComponents();
	void CacheAnimalStatusWidget();
	void BindNeedsEvents();
	
	void SpawnAnimalWaste();
	FVector GetLocationAboveGround();
	
	void CleanAnimal();
	void Die();
	void ScatterMeshParts();
	
	void GeneratePointsIfAnimalIsHappy();
	void GeneratePenaltyPointsWhenAnimalDied();
};
