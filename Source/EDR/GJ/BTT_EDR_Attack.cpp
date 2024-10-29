// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EDR_Attack.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UBTT_EDR_Attack::UBTT_EDR_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTT_EDR_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super:: ExecuteTask(OwnerComp, NodeMemory);

	// 캐릭터 캐스팅
	auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyCharacter)
	{
		return EBTNodeResult::Failed;
	}

	// 공격함수 호출
	MyCharacter->Attack();
	IsAttacking = true;
	// 공격 끝났음을 알림
	MyCharacter->OnAttackEnd.AddLambda([this]() -> void { IsAttacking = false; });
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("over"));
	return EBTNodeResult::InProgress;
}

void UBTT_EDR_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}