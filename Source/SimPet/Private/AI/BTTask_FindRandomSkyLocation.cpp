// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTTask_FindRandomSkyLocation.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomSkyLocation::UBTTask_FindRandomSkyLocation()
{
	NodeName = "Find Random Sky Location";
	
	FlyHeightMin = 300.0f;
	FlyHeightMax = 600.0f;
	
	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomSkyLocation, HomeLocationKey));
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomSkyLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_FindRandomSkyLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
	UBlackboardComponent *BB = OwnerComp.GetBlackboardComponent();
	APawn *Pawn = OwnerComp.GetAIOwner()->GetPawn();
	
	// 1. Отримуємо центр пошуку (Дім)
	FVector Origin = BB->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	if (Origin.IsZero() && Pawn)
	{
		Origin = Pawn->GetActorLocation();
	}
	
	// 2. Рахуємо випадкову точку
	FVector RandomOffset = FMath::VRand();
	RandomOffset.Z = 0;
	RandomOffset.Normalize();
	
	float ActualFlyRadius = BB->GetValueAsFloat(FlyRadiusKey.SelectedKeyName);
	RandomOffset *= FMath::RandRange(0.0f, ActualFlyRadius);
	
	float RandomZ = FMath::RandRange(FlyHeightMin, FlyHeightMax);  // Додаємо випадкову висоту
	
	FVector DestLocation = Origin + RandomOffset;
	DestLocation.Z = Origin.Z + RandomZ;  // Висота відносно рівня землі "Дому"
	
	// 3. Записуємо в Blackboard
	BB->SetValueAsVector(GetSelectedBlackboardKey(), DestLocation);
	
	return EBTNodeResult::Succeeded;
}
