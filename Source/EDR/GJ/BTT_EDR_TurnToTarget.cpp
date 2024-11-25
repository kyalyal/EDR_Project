// Fill out your copyright notice in the Description page of Project Settings

#include "BTT_EDR_TurnToTarget.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "EDR/YS/EDRCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_EDR_TurnToTarget::UBTT_EDR_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTT_EDR_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// MyCharacter 캐스팅
	auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("MyCharacter NULL"));
		return EBTNodeResult::Failed;
	}

	// target 캐스팅
	auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
	if (nullptr == Target)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Target NULL"));
		return EBTNodeResult::Failed;
	}

	// target과 mycharacter의 차이 벡터값
	FVector LookVecter = Target->GetActorLocation() - MyCharacter->GetActorLocation();

	// 로케이션 변경
	LookVecter.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVecter).Rotator();
	MyCharacter->SetActorRotation(FMath::RInterpTo(MyCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));


	return EBTNodeResult::Succeeded;
}