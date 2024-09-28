// Fill out your copyright notice in the Description page of Project Settings.

#include "BTS_EDR_Detect.h"
#include "EDR/YS/EDRPlayerCharacter.h"
#include "MyCharacter.h"
#include"Enemy_EDR_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

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
	float DetectRadius = 600.0f;

	if (nullptr == World)
	{
		return;
	}

	//TArray<FOverlapResult> OverlapResult;
	//FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	//bool bResult = World->OverlapMultiByChannel(
	//	OverlapResult,
	//	Center,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	FCollisionShape::MakeSphere(DetectRadius),
	//	CollisionQueryParam
	//);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}