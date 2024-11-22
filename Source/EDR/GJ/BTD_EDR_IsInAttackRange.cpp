// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_EDR_IsInAttackRange.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"
#include "EDR/YS/EDRCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_EDR_IsInAttackRange::UBTD_EDR_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

void UBTD_EDR_IsInAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// AIContorller가 제어하는 폰을 가져옴
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}


	// 실시간 거리 조건 재평가
	CalculateRawConditionValue(OwnerComp, NodeMemory);
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

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	if (nullptr == World)
	{
		return false;
	}


	// 블랙보드의 Target 플레이어 캐스팅
	auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
	if (nullptr == Target)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("over"));
		return false;
	}

	// 거리 조건 계산
	float Range = 400.0f;
	bResult = (Target->GetDistanceTo(ControllingPawn) <= Range);

	DrawDebugSphere(World, Center, 400.0f, 16, FColor::Red, false, 10.2f);
	
	return bResult;
}
