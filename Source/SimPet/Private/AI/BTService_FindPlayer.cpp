// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SimPetDebugHelper.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	DetectionRange = 800.0f;
		
	NodeName = TEXT("Find Player Service");
	Interval = 0.5f;  // Перевірка кожні 0.5 сек
	RandomDeviation = 0.1f;  // Щоб всі AI не рахували в один кадр
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController ? AIController->GetPawn() : nullptr;
	
	if (!AIPawn)
		return;
	
	ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (Player)
	{
		float Dist = FVector::Dist(AIPawn->GetActorLocation(), Player->GetActorLocation());
		
		if (Dist < DetectionRange)
		{// Якщо дистанція дозволяє - записуємо в BB
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
			Debug::Print(TEXT("if (Dist < DetectionRange)"));
		}
		else
		{// Якщо дистанція не дозволяє - стираємо запис
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}
}
