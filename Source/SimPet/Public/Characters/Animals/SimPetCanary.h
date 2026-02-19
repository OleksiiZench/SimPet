// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Characters/Animals/SimPetAnimal.h"

#include "SimPetCanary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPET_API ASimPetCanary : public ASimPetAnimal
{
	GENERATED_BODY()

public:
	ASimPetCanary();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform &Transform) override;
	
	void SetFlyingMovementMode(const bool IsFlying);

protected:
#pragma region BodyParts
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	bool bHasWings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMesh *WingsAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimPet|BodyParts")
	FName WingsSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SimPet|BodyParts")
	UStaticMeshComponent *SpawnedWings;
#pragma endregion
	
private:
	void AnimateWings(float DeltaTime);
	
	float WingsLiftOffset;
};
