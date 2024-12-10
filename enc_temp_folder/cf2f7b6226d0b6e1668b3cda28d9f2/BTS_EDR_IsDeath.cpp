// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_EDR_IsDeath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "Enemy_EDR_AIController.h"
#include "Enemy_EDR_JAIController.h"
#include "EDR/YS/EDRGameMode.h"
UBTS_EDR_IsDeath::UBTS_EDR_IsDeath()
{

}
void UBTS_EDR_IsDeath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    UWorld* World = ControllingPawn->GetWorld();
    if (nullptr == ControllingPawn)
    {
        return;
    }
    if (nullptr == World)
    {
        return;
    }

    auto MyCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == MyCharacter)
    {
        return;
    }

    // ���Ӹ�� ĳ����
    AEDRGameMode* GameMode = Cast<AEDRGameMode>(World->GetAuthGameMode());


        if (MyCharacter)
        {
            // Death ������ true���� Ȯ��
            bIsDeath = MyCharacter->Death; // Death ������ public�̾�� ��
            if (!bDeathCheck && bIsDeath)
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::Death, bIsDeath);
                // ���Ӹ�尡 ���� �ƴҶ�
                if (GameMode)
                {
                    // EFightMode FightMode�� ����
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FightModeOFF!!!!!!!!!!!!!!!!!!"));
                    GameMode->SetFightMode(EFightMode::None);

                    bDeathCheck = true;
                }
            }
    }
}

