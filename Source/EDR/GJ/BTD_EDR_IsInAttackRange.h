// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_EDR_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UBTD_EDR_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	UBTD_EDR_IsInAttackRange();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override; 
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8* NodeMemory) const override;
};
