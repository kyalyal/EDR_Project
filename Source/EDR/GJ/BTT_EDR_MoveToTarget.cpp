// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_EDR_MoveToTarget.h"
#include "Enemy_EDR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardData.h"
#include "BlackBoardKeys.h"
#include "MyCharacter.h"
#include "EDR/YS/EDRCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

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
    GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, TEXT("MoveToTargetOn"));
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    auto AIController = OwnerComp.GetAIOwner();
    auto ControlledPawn = AIController ? AIController->GetPawn() : nullptr;
    if (!ControlledPawn) return;

    auto MyCharacter = Cast<AMyCharacter>(ControlledPawn);
    if (!MyCharacter) return;

    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return;

    FVector TargetLocation = BlackboardComp->GetValueAsVector(AEnemy_EDR_AIController::TargetLocation);
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    float DistanceToTarget = FVector::Dist(TargetLocation, CurrentLocation);

    // ����� �޽���
    GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Cyan, FString::Printf(TEXT("DistanceToTarget: %f"), DistanceToTarget));
    GEngine->AddOnScreenDebugMessage(6, 2.f, FColor::Orange, FString::Printf(TEXT("StopDistance: %f"), MyCharacter->StopDistance));
    GEngine->AddOnScreenDebugMessage(7, 2.f, FColor::Purple, FString::Printf(TEXT("TargetLocation: %s"), *TargetLocation.ToString()));
    GEngine->AddOnScreenDebugMessage(8, 2.f, FColor::Purple, FString::Printf(TEXT("CurrentLocation: %s"), *CurrentLocation.ToString()));

    // ��ǥ ������ ������ ���
    if (DistanceToTarget <= MyCharacter->StopDistance)
    {
        GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Blue, TEXT("asdfjabsdkjnabsdkmasbdfkasdgkjamsdbaksjdfbakjhb"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); // �½�ũ ����
        return; // ���� �� �߰� ó�� ����
    }

    // ĳ���� �̵� ó��
    if (MyCharacter->GetCharacterMovement())
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = MyCharacter->TargetSpeed;
    }

    MyCharacter->AddMovementInput(Direction, 1.0f);

    // ĳ���� ȸ�� ó��
    Direction.Z = 0.0f;
    FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    FRotator NewRotation = FMath::RInterpTo(ControlledPawn->GetActorRotation(), TargetRotation, DeltaSeconds, MyCharacter->RotationSpeed);
    ControlledPawn->SetActorRotation(NewRotation);

    GEngine->AddOnScreenDebugMessage(4, 5.f, FColor::Yellow, FString::Printf(TEXT("Moving to: %s"), *TargetLocation.ToString()));
}
