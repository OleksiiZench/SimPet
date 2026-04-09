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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimalDiedSignature, ASimPetAnimal *, DeadAnimal);

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
	
	void ApplyForestState();
	void ApplyOwnerState();
	
	ESimPetAnimalState GetAnimalState();
	
	UPROPERTY()
	FOnAnimalDiedSignature OnAnimalDied;
	
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
	bool bAnimalInForest = false;  // Тимчасова змінна, варто прибрати якщо уже продуманий механізм звідки переміщати тварин
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USimPetNeedsComponent *NeedsComponent;
	
	UPROPERTY()
	USimPetPointsTransactionComponent *PointsTransactionComponent;
	
	UFUNCTION()
	void HandleHappyTick();
	
	UFUNCTION()
	void HandleNeedsDepleted();
	
	UFUNCTION()
	void HandleGotDirty();
	
	UFUNCTION()
	void HandleWasteCleaned();
	
	void InitializeBaseBody();
	void SetupComponents();
	void InitializeAnimalStatusWidget();
	void BindNeedsEvents();
	
	void SpawnAnimalWaste();
	FVector GetLocationAboveGround();
	
	void CleanAnimal();
	void Die();
	void ScatterMeshParts();
	
	void UpdateBlackboardForForest();
	void UpdateBlackboardForOwner();
	void SetBlackboardParam(float BaseRadius, float FlyRadius, float ZigZagRadius, bool bChangeHomeLocation);
	
	void GeneratePointsIfAnimalIsHappy();
	void GeneratePenaltyPointsWhenAnimalDied();
};
