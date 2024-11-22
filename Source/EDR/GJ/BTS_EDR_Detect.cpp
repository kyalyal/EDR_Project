// Fill out your copyright notice in the Description page of Project Settings.
#include "BTS_EDR_Detect.h"
#include "EDR/YS/EDRCharacter.h"
#include "MyCharacter.h"
#include "Enemy_EDR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"
#include "EDR/YS/EDRGameMode.h"

UBTS_EDR_Detect::UBTS_EDR_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTS_EDR_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
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
	// 감지 범위
	float DetectRadius = 1500.0f;

	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	// 다른 액터와 오버랩 되는지 감지
	bool bResult = World->OverlapMultiByChannel(
		// 오버랩 결과 저장
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// 게임모드 캐스팅
	AEDRGameMode* GameMode = Cast<AEDRGameMode>(World->GetAuthGameMode());


	// 오버랩시, 참일경우
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			// 플레이어 캐릭터 캐스팅
			AEDRCharacter* EDRCharacter = Cast<AEDRCharacter>(OverlapResult.GetActor());
			// 감지된 액터가 플레이어일경우
			if (EDRCharacter && EDRCharacter->GetController()->IsPlayerController())
			{
				
				// 게임모드가 널이 아닐때
				if (GameMode)
				{

					// 캐릭터 발견시 실행
					if (GameMode->GetFightMode() != EFightMode::FightMode)
					{
						// EFightMode FightMode로 설정
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FightModeOn!!!!!!!!!!!!!!!!!!"));
						GameMode->SetFightMode(EFightMode::FightMode);

						// 전투 시작 애니메이션 재생 함수
						MyCharacter->FightStart();

					}
				}
				// 디버그 정보 색상 출력
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemy_EDR_AIController::TargetKey, EDRCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, EDRCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), EDRCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				World->GetTimerManager().ClearTimer(ResetFightModeTimerHandle);
				return;
			}
		}
	}


	// FightMode가 None이 아닐경우에 == 추적에서 놓친 상태면 실행
	if (GameMode->GetFightMode() != EFightMode::None)
	{
		if (GameMode && !World->GetTimerManager().IsTimerActive(ResetFightModeTimerHandle))
		{
			// 5초 후에 FightMode를 None으로 변경하는 타이머 설정
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Wait 5sec"));
			World->GetTimerManager().SetTimer(ResetFightModeTimerHandle, [GameMode]() 
				{
				// EFightMode None으로 설정
					GameMode->SetFightMode(EFightMode::None);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("FightMode Off~~~~~~~~~~~ "));
				}, 5.0f, false);			
		}
		// 추적 놓쳤을때 Target 초기화
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemy_EDR_AIController::TargetKey, nullptr);
	}
	// 탐지가 안되었을때 디버그 출력
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}