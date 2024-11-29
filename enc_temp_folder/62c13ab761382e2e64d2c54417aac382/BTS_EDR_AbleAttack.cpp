#include "BTS_EDR_AbleAttack.h"
#include "Enemy_EDR_AIController.h"
#include "Enemy_EDR_JAIController.h"
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

    // AIController가 제어하는 Pawn을 가져옴
    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return;
    }
    auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == MyCharacter)
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
    }    // 거리 조건 계산
    float Range = MyCharacter->AttackRange;
    float DistanceToTarget = Target->GetDistanceTo(ControllingPawn);


    // 목표의 방향 벡터 계산
    FVector ForwardVector = ControllingPawn->GetActorForwardVector();
    FVector ToTarget = (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal();

    // 목표가 AI의 앞쪽에 있는지 확인
    bool bIsInFront = FVector::DotProduct(ForwardVector, ToTarget) > 0.0f;

    // 거리와 방향 조건을 모두 확인
    AbleAttack = bIsInFront && (DistanceToTarget <= Range);
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::AbleAttack, AbleAttack);

    // 구체 그리기
    if (AbleAttack)
    {
        DrawDebugSphere(World, Center + (ForwardVector * 200.0f), 400.0f, 16, FColor::Blue, false, 0.2f);
    }
    else
    {
        DrawDebugSphere(World, Center + (ForwardVector * 200.0f), 400.0f, 16, FColor::Red, false, 0.2f);
    }




}