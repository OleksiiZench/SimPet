// Copyright (c) 2025 Oleksii Zenchenko. All Rights Reserved.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	MaxDistanceFromHome = 500.0f;
	DetectionRange = 500.0f;
	
	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindPlayer, HomeLocationKey));
		
	NodeName = TEXT("Find Player Service");
	Interval = 0.5f;  // Перевірка кожні 0.5 сек
	RandomDeviation = 0.1f;  // Щоб всі AI не рахували в один кадр
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	// 1. Отримання AIController, AIPawn
	AAIController *AIController = OwnerComp.GetAIOwner();
	APawn *AIPawn = AIController ? AIController->GetPawn() : nullptr;
	
	if (!AIPawn)
		return;
	
	// 2. Логіка "повідця" від дому тварини
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	
	FVector MyLocation = AIPawn->GetActorLocation();
	FVector HomeLocation = BlackboardComp->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	
	float DistToHome = FVector::Dist(MyLocation, HomeLocation);
	
	if (DistToHome > MaxDistanceFromHome)
	{
		BlackboardComp->ClearValue(GetSelectedBlackboardKey());
		return;
	}
	
	// 3. Запис гравця, якщо відповідна дистанція
	ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (Player)
	{
		float Dist = FVector::Dist(AIPawn->GetActorLocation(), Player->GetActorLocation());
		
		if (Dist < DetectionRange)
		{// Якщо дистанція дозволяє - записуємо в BB
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
		}
		else
		{// Якщо дистанція не дозволяє - стираємо запис
			OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
		}
	}
}
