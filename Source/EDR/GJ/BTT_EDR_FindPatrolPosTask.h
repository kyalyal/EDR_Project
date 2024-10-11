// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BTT_EDR_FindPatrolPosTask.generated.h"
/**
 * 
 */
UCLASS(Blueprintable)
class EDR_API UBTT_EDR_FindPatrolPosTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_EDR_FindPatrolPosTask(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float search_radius = 1500.f;
};
