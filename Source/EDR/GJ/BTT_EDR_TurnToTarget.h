// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EDR_TurnToTarget.generated.h"

/**
 *
 */
UCLASS()
class EDR_API UBTT_EDR_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_EDR_TurnToTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

};


