// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_EDR_IsInAttackRange.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "EDR/YS/EDRCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_EDR_IsInAttackRange::UBTD_EDR_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}


bool UBTD_EDR_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// AIContorller가 제어하는 폰을 가져옴
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	// 블랙보드의 Target 플레이어 캐스팅
	auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	// 거리 조건 계산
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
