// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_EDR_IsDeath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "Enemy_EDR_AIController.h"
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

    // 게임모드 캐스팅
    AEDRGameMode* GameMode = Cast<AEDRGameMode>(World->GetAuthGameMode());


    // AIController 가져오기
    AEnemy_EDR_AIController* AIController = Cast<AEnemy_EDR_AIController>(OwnerComp.GetOwner());
    if (AIController)
    {
        // Pawn을 MyCharacter로 캐스팅
        AMyCharacter* MyCharacter = Cast<AMyCharacter>(AIController->GetPawn());
        if (MyCharacter)
        {
            // Death 변수가 true인지 확인
            bIsDeath = MyCharacter->Death; // Death 변수가 public이어야 함
            if (!bDeathCheck && bIsDeath)
            {
                OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEnemy_EDR_AIController::Death, bIsDeath);
                // 게임모드가 널이 아닐때
                if (GameMode)
                {
                    // EFightMode FightMode로 설정
                    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FightModeOFF!!!!!!!!!!!!!!!!!!"));
                    GameMode->SetFightMode(EFightMode::None);

                    bDeathCheck = true;
                }
            }
        }
    }
}

