// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EDR_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UBTT_EDR_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EDR_MoveToTarget();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
