// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Giant.h"

AEDR_Boss_Giant::AEDR_Boss_Giant()
{
	// hp ������
	hp = 100.0f;
	AttackDamage = 40.0f;

	// ���� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/HeavyGoundHitR_Montage.HeavyGoundHitR_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	// ��� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Death1_Montage.Death1_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}
}
