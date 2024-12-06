// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Dragon.h"
#include "EDR_Enemy_WeaponRDragon.h"
#include "EDR_Enemy_WeaponLDragon.h"
#include "GameFramework/CharacterMovementComponent.h"

AEDR_Boss_Dragon::AEDR_Boss_Dragon()
{

	IsBoss = true;
	DetectRange = 2500.0f;
	// hp ������
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 800.0f;
	AttackRadius = 500.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	// �ӵ�
	TargetSpeed = 500.0f;
	Acceleration = 300.0f;

	//GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// ĳ���� �޽ö� ĸ���ݸ���
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	// �޽� �޾ƿ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/DesertDragon/Meshes/SK_DesertDragon.SK_DesertDragon"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}

	// �ִϸ��̼� ����

// ���� ���� �ִϸ��̼�

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIGHTSTART_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/ANIM_DesertDragon_FlyStationaryToLanding_Montage.ANIM_DesertDragon_FlyStationaryToLanding_Montage"));
	if (FIGHTSTART_MONTAGE.Succeeded())
	{
		FightStartMontage = FIGHTSTART_MONTAGE.Object;
	}


	// ����
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


	// ���߿� �߰�
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

	// ��ų �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/Attack/ANIM_DesertDragon_2HitComboClawsAttackForward_Montage.ANIM_DesertDragon_2HitComboClawsAttackForward_Montage"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE1(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_01_Cue.LionMonster_Attack_01_Cue"));
	if (ATTACKSOUNDCUE1.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE1.Object);
	}
	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE2(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_02_Cue.LionMonster_Attack_02_Cue"));
	if (ATTACKSOUNDCUE2.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE2.Object);
	}
	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE3(TEXT("/Game/Monster_Sounds_Volume_01/Cues/LionMonster_Attack_03_Cue.LionMonster_Attack_03_Cue"));
	if (ATTACKSOUNDCUE3.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE3.Object);
	}






	// ��� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/DragonEnemy/ANIM_DesertDragon_Death_Montage.ANIM_DesertDragon_Death_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}


}

void AEDR_Boss_Dragon::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����� ĳ���� �տ� ���� ����
	FName WeaponSocket(TEXT("RHandSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_WeaponRDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);

	}

	//// ���� ����� ĳ���� �տ� ���� ����
	FName WeaponSocket2(TEXT("LHandSocket"));
	CurrentWeapon2 = GetWorld()->SpawnActor<AEDR_Enemy_WeaponLDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket2);

	}

	//// ���� ����� ĳ���� �տ� ���� ����
	FName WeaponSocket3(TEXT("HeadSocket"));
	CurrentWeapon3 = GetWorld()->SpawnActor<AEDR_Enemy_WeaponLDragon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket3);

	}
}