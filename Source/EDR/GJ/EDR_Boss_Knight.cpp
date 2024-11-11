// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Knight.h"
#include "EDR_Enemy_Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
AEDR_Boss_Knight::AEDR_Boss_Knight()
{
	// hp ������
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 400.0f;
	AttackRadius = 200.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	GetCharacterMovement()->MaxWalkSpeed = 200.0f;

	// ĳ���� �޽ö� ĸ���ݸ���
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));


	// �޽� �޾ƿ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Characters/Thral/SK_ThrallRigged.SK_ThrallRigged"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}	
	

	// ũ�� 1.6�� Ű���
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));	
	
	
	
	
	// �ִϸ��̼� ����

	// ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_Uppercut_InP_Montage.Boss_Attack_Uppercut_InP_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_Swing_InP_Montage.Boss_Attack_Swing_InP_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE3(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_HandAndSwordSwing_InP_Montage.Boss_Attack_HandAndSwordSwing_InP_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE3.Object);
	}


	// ��ų �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL_MONTAGE(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/Attack/Boss_Attack_ChargedCombo_InP_Montage.Boss_Attack_ChargedCombo_InP_Montage"));
	if (SKILL_MONTAGE.Succeeded())
	{
		SkillMontage = SKILL_MONTAGE.Object;
	}

	// ��� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/KnighEnemy/GiantGolemIdleDeath01_Montage.GiantGolemIdleDeath01_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}

	// ���� ����

	// ���� ��Ʈ�� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE1(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_3_Cue.Blunt_Weapon_Hits_Flesh_3_Cue"));
	if (HITSOUNDCUE1.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE1.Object);
	}	// ���� ��Ʈ�� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE2(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_1_Cue.Blunt_Weapon_Hits_Flesh_1_Cue"));
	if (HITSOUNDCUE2.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE2.Object);
	}	// ���� ��Ʈ�� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>HITSOUNDCUE3(TEXT("/Game/Sword_Fighting_SFX/Cues/Blunt_Weapon_Hits_Flesh_5_Cue.Blunt_Weapon_Hits_Flesh_5_Cue"));
	if (HITSOUNDCUE3.Succeeded())
	{
		HitSoundCue.Add(HITSOUNDCUE1.Object);
	}


}
void AEDR_Boss_Knight::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����� ĳ���� �տ� ���� ����
	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_Weapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180�� ȸ��
		CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // �����̵�


		// ���� ũ�� Ű���
		CurrentWeapon->SetActorScale3D(FVector(2.0f));
	}
}
