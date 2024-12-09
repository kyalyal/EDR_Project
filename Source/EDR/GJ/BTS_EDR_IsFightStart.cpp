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
		// ĳ����
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(AIController->GetPawn());

		if (MyCharacter)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASDFADSF"));
			// ���ݾִϸ��̼� ���������
			bIsFightStartEnd = MyCharacter->IsFightStarting; 
			if (bIsFightStartEnd)
			{
				// ������ ������ ����
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::FightStartEnd, bIsFightStartEnd);
			}
		}
	}
}