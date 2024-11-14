// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_EDR_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UAnim_EDR_AnimInstance::UAnim_EDR_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;


}

// �������� ��ȣ
void UAnim_EDR_AnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

// ���� ���� ��ȣ
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
		//  ���� �ӷ°� �޾ƿ��� �Լ�
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}
