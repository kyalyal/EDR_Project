// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_EDR_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UAnim_EDR_AnimInstance::UAnim_EDR_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;


}

// 공격지점 신호
void UAnim_EDR_AnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

// 공격 스텝 신호
void UAnim_EDR_AnimInstance::AnimNotify_AttackStep()
{
	OnAttackStep.Broadcast();
}


void UAnim_EDR_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		//  폰의 속력값 받아오는 함수
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}
