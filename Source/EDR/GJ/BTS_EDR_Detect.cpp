// Fill out your copyright notice in the Description page of Project Settings.
#include "BTS_EDR_Detect.h"
#include "EDR/YS/EDRCharacter.h"
#include "MyCharacter.h"
#include "Enemy_EDR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

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
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	// 감지 범위
	float DetectRadius = 2000.0f;

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
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	// 오버랩시, 참일경우
	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AEDRCharacter* EDRCharacter = Cast<AEDRCharacter>(OverlapResult.GetActor());
			// 감지된 액터가 플레이어일경우
			if (EDRCharacter && EDRCharacter->GetController()->IsPlayerController())
			{
				// 디버그 정보 색상 출력
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemy_EDR_AIController::TargetKey, EDRCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugPoint(World, EDRCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), EDRCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}