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
	//에디터에 보여질 TaskNode의 이름
	NodeName = TEXT("FindPatrolPosTask");
}


EBTNodeResult::Type UBTT_EDR_FindPatrolPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	auto Controller = Cast<AEnemy_EDR_AIController>(OwnerComp.GetAIOwner());
	auto Enemy = Controller->GetPawn();
	// MyCharacter 캐스팅
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
	//현재 에디터에 설정된 navi mesh로 초기화
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem) {
		//navi mesh가 없다면 실패를 반환
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}

	FVector const Origin = Enemy->GetActorLocation();
	FNavLocation NextPatrol;

	//NextPatrol변수에 임의의 location 데이터를 넣고 다시 TargetLocation키의 value에 값을 넣어준다.
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, search_radius, NextPatrol, nullptr))
	{
		Controller->get_blackboard()->SetValueAsVector(AEnemy_EDR_AIController::TargetLocation, NextPatrol.Location);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NextPosSuccess!"));
	}

	//그 다음 이동할 곳을 확인하기 위한 디버그메시지
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *NextPatrol.Location.ToString()));

	// target과 mycharacter의 차이 벡터값
	FVector LookVecter = NextPatrol.Location - MyCharacter->GetActorLocation();

	// 로케이션 변경
	LookVecter.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVecter).Rotator();
	MyCharacter->SetActorRotation(FMath::RInterpTo(MyCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}