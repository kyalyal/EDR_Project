// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Dragon.h"
#include "EDR_Enemy_WeaponRDragon.h"
#include "EDR_Enemy_WeaponLDragon.h"
#include "GameFramework/CharacterMovementComponent.h"

AEDR_Boss_Dragon::AEDR_Boss_Dragon()
{

	IsBoss = true;
	DetectRange = 2500.0f;
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 800.0f;
	AttackRadius = 500.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	// 속도
	TargetSpeed = 500.0f;
	Acceleration = 300.0f;

	//GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 캐릭터 메시랑 캡슐콜리전
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	// 메시 받아오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/DesertDragon/Meshes/SK_DesertDragon.SK_DesertDragon"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}

	// 애니메이션 관련

// 전투 시작 애니메이션

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIGHTSTART_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/ANIM_DesertDragon_FlyStationaryToLanding_Montage.ANIM_DesertDragon_FlyStationaryToLanding_Montage"));
	if (FIGHTSTART_MONTAGE.Succeeded())
	{
		FightStartMontage = FIGHTSTART_MONTAGE.Object;
	}


	// 공격
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_RM_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_BiteAttack_RM_Montage.ANIM_DesertDragon_BiteAttack_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE3.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE4(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_ClawsAttackLeftForward_RM_Montage.ANIM_DesertDragon_ClawsAttackLeftForward_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE4.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE5(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_ClawsAttackRightForward_RM_Montage.ANIM_DesertDragon_ClawsAttackRightForward_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE5.Object);
	}


	// 나중에 추가
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE6(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE6.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE7(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE7.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE8(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE8.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE9(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE9.Object);
	}

	// 스킬 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE1(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_01_Cue.LionMonster_Attack_01_Cue"));
	if (ATTACKSOUNDCUE1.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE1.Object);
	}
	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE2(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_02_Cue.LionMonster_Attack_02_Cue"));
	if (ATTACKSOUNDCUE2.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE2.Object);
	}
	// 공격 사운드 저장
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE3(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_03_Cue.LionMonster_Attack_03_Cue"));
	if (ATTACKSOUNDCUE3.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE3.Object);
	}






	// 사망 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/ANIM_DesertDragon_Death_Montage.ANIM_DesertDragon_Death_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}


}

void AEDR_Boss_Dragon::BeginPlay()
{
	Super::BeginPlay();

	// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket(TEXT("RHandSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_WeaponRDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

	}

	//// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket2(TEXT("LHandSocket"));
	CurrentWeapon2 = GetWorld()->SpawnActor<AEDR_Enemy_WeaponLDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);

	}

	//// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket3(TEXT("HeadSocket"));
	CurrentWeapon3 = GetWorld()->SpawnActor<AEDR_Enemy_WeaponLDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket3);

	}
}