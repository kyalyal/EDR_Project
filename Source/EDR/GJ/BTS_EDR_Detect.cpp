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
	// ���� ����
	float DetectRadius = 1500.0f;

	if (nullptr == World)
	{
		return;
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	// �ٸ� ���Ϳ� ������ �Ǵ��� ����
	bool bResult = World->OverlapMultiByChannel(
		// ������ ��� ����
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// ���Ӹ�� ĳ����
	AEDRGameMode* GameMode = Cast<AEDRGameMode>(World->GetAuthGameMode());


	// ��������, ���ϰ��
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			// �÷��̾� ĳ���� ĳ����
			AEDRCharacter* EDRCharacter = Cast<AEDRCharacter>(OverlapResult.GetActor());
			// ������ ���Ͱ� �÷��̾��ϰ��
			if (EDRCharacter && EDRCharacter->GetController()->IsPlayerController())
			{
				
				// ���Ӹ�尡 ���� �ƴҶ�
				if (GameMode)
				{

					// ĳ���� �߽߰� ����
					if (GameMode->GetFightMode() != EFightMode::FightMode)
					{
						// EFightMode FightMode�� ����
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FightModeOn!!!!!!!!!!!!!!!!!!"));
						GameMode->SetFightMode(EFightMode::FightMode);

						// ���� ���� �ִϸ��̼� ��� �Լ�
						MyCharacter->FightStart();

					}
				}
				// ����� ���� ���� ���
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemy_EDR_AIController::TargetKey, EDRCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, EDRCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), EDRCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				World->GetTimerManager().ClearTimer(ResetFightModeTimerHandle);
				return;
			}
		}
	}


	// FightMode�� None�� �ƴҰ�쿡 == �������� ��ģ ���¸� ����
	if (GameMode->GetFightMode() != EFightMode::None)
	{
		if (GameMode && !World->GetTimerManager().IsTimerActive(ResetFightModeTimerHandle))
		{
			// 5�� �Ŀ� FightMode�� None���� �����ϴ� Ÿ�̸� ����
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Wait 5sec"));
			World->GetTimerManager().SetTimer(ResetFightModeTimerHandle, [GameMode]() 
				{
				// EFightMode None���� ����
					GameMode->SetFightMode(EFightMode::None);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("FightMode Off~~~~~~~~~~~ "));
				}, 5.0f, false);			
		}
		// ���� �������� Target �ʱ�ȭ
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemy_EDR_AIController::TargetKey, nullptr);
	}
	// Ž���� �ȵǾ����� ����� ���
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}