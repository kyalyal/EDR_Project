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

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
