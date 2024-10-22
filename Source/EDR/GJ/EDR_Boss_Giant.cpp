// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Giant.h"

AEDR_Boss_Giant::AEDR_Boss_Giant()
{
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 400.0f;
	AttackRadius = 200.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;

	// 공격 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/HeavyGoundHitR_Montage.HeavyGoundHitR_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	// 공격 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/SmashAttack2_Montage.SmashAttack2_Montage"));
	if (ATTACK_MONTAGE2.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}
	// 스킬 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/GroundFallAttack_Montage.GroundFallAttack_Montage"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	// 사망 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Death1_Montage.Death1_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}

	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE(TEXT("/Game/Monster_Sounds_Volume_01/Cues/Ancestral_Monster_Attack_01_Cue.Ancestral_Monster_Attack_01_Cue"));
	if (ATTACKSOUNDCUE.Succeeded())
	{
		AttackSoundCue = ATTACKSOUNDCUE.Object;
	}

	// 스킬 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>SKILLSOUNDCUE(TEXT("/Game/Monster_Sounds_Volume_01/Cues/Angry_Shout_Cue.Angry_Shout_Cue"));
	if (SKILLSOUNDCUE.Succeeded())
	{
		SkillSoundCue = SKILLSOUNDCUE.Object;
	}
}
