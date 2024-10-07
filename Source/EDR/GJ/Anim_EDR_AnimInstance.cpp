// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_EDR_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UAnim_EDR_AnimInstance::UAnim_EDR_AnimInstance()
{
	CurrentPawnSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/SwordAttack_Montage.SwordAttack_Montage"));
	if (ATTACK_MONTAGE.Succeeded()) 
	{
		
		AttackMontage = ATTACK_MONTAGE.Object;
	}
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

void UAnim_EDR_AnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("hahahahahah"));
		Montage_Play(AttackMontage, 1.0f);
	}
}