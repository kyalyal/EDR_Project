// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EDR_MoveToTarget.h"
#include "Enemy_EDR_AIController.h"
#include "MyCharacter.h"
#include "EDR/YS/EDRCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_EDR_MoveToTarget::UBTT_EDR_MoveToTarget()
{
    bNotifyTick = true;
}
EBTNodeResult::Type UBTT_EDR_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
    return EBTNodeResult::InProgress;
}
void UBTT_EDR_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    auto AIController = OwnerComp.GetAIOwner();
    auto ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    if (!ControlledPawn) return;

    auto MyCharacter = Cast<AMyCharacter>(ControlledPawn);
    if (!MyCharacter) return;

    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;
    FVector TargetLocation = BlackboardComp->GetValueAsVector(AEnemy_EDR_AIController::TargetLocation);
    auto Target = Cast<AEDRCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemy_EDR_AIController::TargetKey));
    if (nullptr != Target)
    {
        TargetLocation = Target->GetActorLocation();
    }
    // ��ǥ ������ ������ ���
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    float DistanceToTarget = FVector::Dist(TargetLocation, CurrentLocation);

    if (DistanceToTarget <= MyCharacter->StopDistance)
    {
        MyCharacter->TargetSpeed = 0.0f; // ��ǥ �ӵ��� 0���� ����
        GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, TEXT("Reached Target."));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // �½�ũ ����
        return; // ���� �� �߰� ó�� ����
    }
    else
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MyCharacter->TargetSpeed;
    }

    // �ӵ� ����
    MyCharacter->CurrentSpeed = FMath::FInterpTo(MyCharacter->CurrentSpeed, MyCharacter->TargetSpeed, DeltaSeconds, MyCharacter->Acceleration); // Acceleration�� ������ ������ ����

    // ĳ���� �̵� ó��
    if (MyCharacter->GetCharacterMovement())
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MyCharacter->CurrentSpeed;
    }

    MyCharacter->AddMovementInput(Direction, 1.0f);

    // ĳ���� ȸ�� ó��
    Direction.Z = 0.0f;
    FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetRotation, DeltaSeconds, MyCharacter->RotationSpeed);
    ControlledPawn->SetActorRotation(NewRotation);

    GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("Moving to: %s"), *TargetLocation.ToString()));
}