// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Knight.h"
#include "EDR_Enemy_BWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
AEDR_Boss_Knight::AEDR_Boss_Knight()
{
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 400.0f;
	AttackRadius = 200.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	//GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// 캐릭터 메시랑 캡슐콜리전
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	// 메시 받아오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Characters/Thral/SK_ThrallRigged.SK_ThrallRigged"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}	
	

	// 크기 1.6배 키우기
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));	
	
	
	
	
	// 애니메이션 관련

	// 전투 시작 애니메이션

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIGHTSTART_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/GJ/Animation/Enemy_Anim/KnighEnemy/NoneBattle/Boss_BattleEntrance_RM_Montage.Boss_BattleEntrance_RM_Montage'"));
	if (FIGHTSTART_MONTAGE.Succeeded())
	{
		FightStartMontage = FIGHTSTART_MONTAGE.Object;
	}


	// 공격
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Anim_SwordV2_Combo2_1_Montage.Anim_SwordV2_Combo2_1_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_Running_Simple_RM_Montage.Boss_Attack_Running_Simple_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_Swing_RM_Montage.Boss_Attack_Swing_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE3.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE4(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_SwingAndSlam_RM_Montage.Boss_Attack_SwingAndSlam_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE4.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE5(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_Uppercut_RM_Montage.Boss_Attack_Uppercut_RM_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE5.Object);
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE6(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Anim_SwordV2_Combo2_Montage.Anim_SwordV2_Combo2_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE6.Object);
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE7(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Anim_Sword_1H_Attack_Jump3_Montage.Anim_Sword_1H_Attack_Jump3_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE7.Object);
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE8(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Anim_SwordV2_Combo1_1_Montage.Anim_SwordV2_Combo1_1_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE8.Object);
	}	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE9(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Anim_SwordV2_Combo1_Montage.Anim_SwordV2_Combo1_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE9.Object);
	}


	// 스킬 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_ChargedComboAttack_RM_Montage.Boss_Attack_ChargedComboAttack_RM_Montage"));
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
void AEDR_Boss_Knight::BeginPlay()
{
	Super::BeginPlay();

	// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_BWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180도 회전
		CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // 무기이동



	}
}
