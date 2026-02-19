// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SimPetDebugRadius.generated.h"

UCLASS()
class SIMPET_API ASimPetDebugRadius : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimPetDebugRadius();
	
	virtual void Tick(float DeltaTime) override;
	virtual bool ShouldTickIfViewportsOnly() const override;
	
	UPROPERTY(EditAnywhere, Category = "Debug Settings")
	bool isShow;
	
	UPROPERTY(EditAnywhere, Category = "Debug Settings", meta = (ClampMin = "0.0"))
	float Radius;
	
	UPROPERTY(EditAnywhere, Category = "Debug Settings")
	FColor LineColor;
	
	UPROPERTY(EditAnywhere, Category = "Debug Settings", meta = (ClampMin = "0.1"))
	float LineThickness;
	
	UPROPERTY(EditAnywhere, Category = "Debug Settings", meta = (ClampMin = "4"))
	int32 Segments;
};
