// Fill out your copyright notice in the Description page of Project Settings.
#include "BTS_EDR_AbleAttack.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"
#include "EDR/YS/EDRCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_EDR_AbleAttack::UBTS_EDR_AbleAttack()
{

}
void UBTS_EDR_AbleAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// AIContorller가 제어하는 폰을 가져옴
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	if (nullptr == World)
	{
		return;
	}	
	// 블랙보드의 Target 플레이어 캐스팅
	auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
	if (nullptr == Target)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("over"));
		return;
	}

	// 거리 조건 계산
	float Range = 500.0f;
	AbleAttack = Target->GetDistanceTo(ControllingPawn) <= Range;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::AbleAttack, AbleAttack);
	if (AbleAttack)
	{
		DrawDebugSphere(World, Center, 400.0f, 16, FColor::Blue, false, 0.2f);
	}
	else
	{
		DrawDebugSphere(World, Center, 400.0f, 16, FColor::Red, false, 0.2f);
	}
	
}