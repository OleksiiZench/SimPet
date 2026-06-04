#include "Save/Structures/SimPetAnimalSaveData.h"

FSimPetAnimalSaveData::FSimPetAnimalSaveData()
{
	Transform = FTransform::Identity;
	TimeSinceLastMeal = 0.0f;
	TimeSinceLastClean = 0.0f;
	bIsOwned = false;
}
