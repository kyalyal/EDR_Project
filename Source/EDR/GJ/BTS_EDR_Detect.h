// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_EDR_Detect.generated.h"
/**
 * 
 */
UCLASS()
class EDR_API UBTS_EDR_Detect : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_EDR_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;


	
};
