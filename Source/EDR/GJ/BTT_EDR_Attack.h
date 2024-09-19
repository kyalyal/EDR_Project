// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EDR_Attack.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UBTT_EDR_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	

public:


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


private:

	TObjectPtr<class AMyCharacter> Monster;


};
