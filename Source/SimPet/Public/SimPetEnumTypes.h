#pragma once

UENUM(BlueprintType)
enum class ESimPetAnimalState : uint8
{
	Happy		UMETA(DisplayName = "Happy"),
	Tired		UMETA(DisplayName = "Tired"),
	Dead		UMETA(DisplayName = "Dead")
};
