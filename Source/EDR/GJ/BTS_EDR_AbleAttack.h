// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_EDR_AbleAttack.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UBTS_EDR_AbleAttack : public UBTService
{
	GENERATED_BODY()
public:

	UBTS_EDR_AbleAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	bool AbleAttack = false;
};
