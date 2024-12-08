// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EDR_FindPatrolPosTask.h"
#include "Enemy_EDR_AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BlackBoardKeys.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_EDR_FindPatrolPosTask::UBTT_EDR_FindPatrolPosTask(FObjectInitializer const& object_initializer)
{
	//�����Ϳ� ������ TaskNode�� �̸�
	NodeName = TEXT("FindPatrolPosTask");
}


EBTNodeResult::Type UBTT_EDR_FindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	auto Controller = Cast<AEnemy_EDR_AIController>(OwnerComp.GetAIOwner());
	auto Enemy = Controller->GetPawn();
	// MyCharacter ĳ����
	auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("MyCharacter NULL"));
		return EBTNodeResult::Failed;
	}
	if (nullptr == Enemy) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Initialize faild!"));
		return EBTNodeResult::Failed;
	}
	//���� �����Ϳ� ������ navi mesh�� �ʱ�ȭ
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem) {
		//navi mesh�� ���ٸ� ���и� ��ȯ
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}

	FVector const Origin = Enemy->GetActorLocation();
	FNavLocation NextPatrol;

	//NextPatrol������ ������ location �����͸� �ְ� �ٽ� TargetLocationŰ�� value�� ���� �־��ش�.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, search_radius, NextPatrol, nullptr))
	{
		Controller->get_blackboard()->SetValueAsVector(AEnemy_EDR_AIController::TargetLocation, NextPatrol.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NextPosSuccess!"));
	}

	//�� ���� �̵��� ���� Ȯ���ϱ� ���� ����׸޽���
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *NextPatrol.Location.ToString()));

	// target�� mycharacter�� ���� ���Ͱ�
	FVector LookVecter = NextPatrol.Location - MyCharacter->GetActorLocation();

	// �����̼� ����
	LookVecter.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVecter).Rotator();
	MyCharacter->SetActorRotation(FMath::RInterpTo(MyCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}