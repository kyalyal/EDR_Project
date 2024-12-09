// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Giant.h"
#include "EDR_Enemy_WeaponSpear.h"
AEDR_Boss_Giant::AEDR_Boss_Giant()
{
	DetectRange = 2000.0f;
	IsBoss = true; 
	// hp 재정의
	hp = 1000.0f;
	AttackDamage = 40.0f;
	AttackRange = 400.0f;
	AttackRadius = 200.0f;


	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	// 속도
	TargetSpeed = 0.0f;
	Acceleration = 15.0f;
	RotationSpeed = 1.0f;
	Deceleration = 280.0f;
	StopDistance = 400.0f;
	MaxWalkSpeed = 400.0f;


	//GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 캐릭터 메시랑 캡슐콜리전
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(80.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -80.0f), FRotator(0.0f, -90.0f, 0.0f));


	// 메시 받아오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Characters/A_Sanpet/SK_SanpetPreRig.SK_SanpetPreRig"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}

	// 크기 1.6배 키우기
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));




	// 애니메이션 관련

	// 전투 시작 애니메이션

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIGHTSTART_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/NoneBattle/Boss_BattleEntrance_RM_Montage.Boss_BattleEntrance_RM_Montage"));
	if (FIGHTSTART_MONTAGE.Succeeded())
	{
		FightStartMontage = FIGHTSTART_MONTAGE.Object;
	}


	// 공격
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossAirSlashAttack_Montage.BossAirSlashAttack_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossJumpingAttack_Montage.BossJumpingAttack_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossAirSlashAttackCombo_Montage.BossAirSlashAttackCombo_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE3.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE4(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossSpearComboAttack_Montage.BossSpearComboAttack_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE4.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE5(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossTurningAttackSingle_Montage.BossTurningAttackSingle_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE5.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE6(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossTurningAttack1_Montage.BossTurningAttack1_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE6.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE7(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossDodgeAttack_Montage.BossDodgeAttack_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE7.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE8(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossDodgeB_Montage.BossDodgeB_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE8.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE9(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossSpearStab_Montage.BossSpearStab_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE9.Object);
	}


	// 스킬 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Attack/BossSpinAttack_Montage.BossSpinAttack_Montage"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	// 사망 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/GiantGolemIdleDeath01_Montage.GiantGolemIdleDeath01_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}

	// 사운드 관련

	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE1(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_1_Cue.Low_Furious_VOB_P2_1_Cue"));
	if (ATTACKSOUNDCUE1.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE1.Object);
	}
	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE2(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_54_Cue.Low_Furious_VOB_P2_54_Cue"));
	if (ATTACKSOUNDCUE2.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE2.Object);
	}
	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE3(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_2_Cue.Low_Furious_VOB_P2_2_Cue"));
	if (ATTACKSOUNDCUE3.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE3.Object);
	}


	// 공격 히트시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE1(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_3_Cue.Blunt_Weapon_Hits_Flesh_3_Cue"));
	if (HITSOUNDCUE1.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE1.Object);
	}	// 공격 히트시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE2(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_1_Cue.Blunt_Weapon_Hits_Flesh_1_Cue"));
	if (HITSOUNDCUE2.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE2.Object);
	}	// 공격 히트시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE3(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_5_Cue.Blunt_Weapon_Hits_Flesh_5_Cue"));
	if (HITSOUNDCUE3.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE1.Object);
	}


	// 공격 미스시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>MISSSOUNDCUE1(TEXT("/Game/Sword_Fighting_SFX/Cues/Sword_Hit_1_Cue.Sword_Hit_1_Cue"));
	if (MISSSOUNDCUE1.Succeeded())
	{
		MissSoundCue.Add(MISSSOUNDCUE1.Object);
	}	// 공격 미스시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>MISSSOUNDCUE2(TEXT("/Game/Sword_Fighting_SFX/Cues/Sword_Hit_1_Cue.Sword_Hit_2_Cue"));
	if (MISSSOUNDCUE2.Succeeded())
	{
		MissSoundCue.Add(MISSSOUNDCUE2.Object);
	}	// 공격 미스시 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>MISSSOUNDCUE3(TEXT("/Game/Sword_Fighting_SFX/Cues/Sword_Hit_1_Cue.Sword_Hit_5_Cue"));
	if (MISSSOUNDCUE3.Succeeded())
	{
		MissSoundCue.Add(MISSSOUNDCUE3.Object);
	}

}

void AEDR_Boss_Giant::BeginPlay()
{
	Super::BeginPlay();

	// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket(TEXT("hand_rSpear"));
	CurrentWeapon = (GetWorld()->SpawnActor<AEDR_Enemy_WeaponSpear>(FVector::ZeroVector, FRotator::ZeroRotator));
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180도 회전
		CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // 무기이동



	}
}

