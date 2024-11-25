// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_EDR_IsFightStart.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UBTS_EDR_IsFightStart : public UBTService
{
	GENERATED_BODY()
	UBTS_EDR_IsFightStart();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	bool bIsFightStartEnd;
};