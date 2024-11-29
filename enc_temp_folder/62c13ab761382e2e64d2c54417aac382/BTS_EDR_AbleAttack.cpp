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

    // AIController�� �����ϴ� Pawn�� ������
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

    // �������� Target �÷��̾� ĳ����

    auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
    if (nullptr == Target)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("over"));
        return;
    }    // �Ÿ� ���� ���
    float Range = MyCharacter->AttackRange;
    float DistanceToTarget = Target->GetDistanceTo(ControllingPawn);


    // ��ǥ�� ���� ���� ���
    FVector ForwardVector = ControllingPawn->GetActorForwardVector();
    FVector ToTarget = (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).GetSafeNormal();

    // ��ǥ�� AI�� ���ʿ� �ִ��� Ȯ��
    bool bIsInFront = FVector::DotProduct(ForwardVector, ToTarget) > 0.0f;

    // �Ÿ��� ���� ������ ��� Ȯ��
    AbleAttack = bIsInFront && (DistanceToTarget <= Range);
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::AbleAttack, AbleAttack);

    // ��ü �׸���
    if (AbleAttack)
    {
        DrawDebugSphere(World, Center + (ForwardVector * 200.0f), 400.0f, 16, FColor::Blue, false, 0.2f);
    }
    else
    {
        DrawDebugSphere(World, Center + (ForwardVector * 200.0f), 400.0f, 16, FColor::Red, false, 0.2f);
    }




}