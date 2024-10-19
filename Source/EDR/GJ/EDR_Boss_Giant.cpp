// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Giant.h"

AEDR_Boss_Giant::AEDR_Boss_Giant()
{
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 40.0f;

	// 공격 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/HeavyGoundHitR_Montage.HeavyGoundHitR_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// 사망 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Death1_Montage.Death1_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}
}
