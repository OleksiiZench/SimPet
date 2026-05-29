#include "Save/Structures/SimPetPlayerSaveData.h"


FSimPetPlayerSaveData::FSimPetPlayerSaveData()
{
	Transform = FTransform::Identity;
	Stamina = 0.0f;
	ControlRotation = FRotator::ZeroRotator;
}
