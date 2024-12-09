// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_EDR_IsFightStart.h"
#include "Enemy_EDR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Mycharacter.h"
UBTS_EDR_IsFightStart::UBTS_EDR_IsFightStart()
{
	bIsFightStartEnd = false;
}
void UBTS_EDR_IsFightStart::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AEnemy_EDR_AIController* AIController = Cast<AEnemy_EDR_AIController>(OwnerComp.GetOwner());
	if (AIController)
	{
		// 캐스팅
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(AIController->GetPawn());

		if (MyCharacter)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASDFADSF"));
			// 공격애니메이션 재생중인지
			bIsFightStartEnd = MyCharacter->IsFightStarting; 
			if (bIsFightStartEnd)
			{
				// 블랙보드 변수에 저장
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::FightStartEnd, bIsFightStartEnd);
			}
		}
	}
}