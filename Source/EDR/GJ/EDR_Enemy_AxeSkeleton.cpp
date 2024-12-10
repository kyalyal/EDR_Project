// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_AxeSkeleton.h"

#include "EDR_Enemy_SWeaponAxe.h"

AEDR_Enemy_AxeSkeleton::AEDR_Enemy_AxeSkeleton()
{
	IsBoss = false;
	DetectRange = 1500.0f;

	IsFightStarting = true;


	// hp ������
	hp = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 300.0f;
	AttackRadius = 150.0f;

	SkillDamage = 20.0f;
	SkillRange = 300.0f;
	SkillRadius = 150.0f;

	// �ӵ�
	TargetSpeed = 0.0f;
	Acceleration = 10.0f;
	Deceleration = 200.0f;
	RotationSpeed = 1.0f;
	StopDistance = 150.0f;
	MaxWalkSpeed = 300.0f;
	// ĳ���� �޽ö� ĸ���ݸ���
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// �޽� �޾ƿ���
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/SKnight_modular/Skeleton_Knight_04/mesh/SK_SKnight_full_04.SK_SKnight_full_04"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}


	// ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Axe/Attack/Anim_DK_Combo_A03_RM_Montage.Anim_DK_Combo_A03_RM_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Axe/Attack/Anim_Katana_Attack_Slash1_Montage.Anim_Katana_Attack_Slash1_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}

	// ��� �ִϸ��̼� ��Ÿ�� ����
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Axe/Deaths_Hit_Head_Spinning_Montage.Deaths_Hit_Head_Spinning_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}


	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE1(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_1_Cue.Low_Furious_VOB_P2_1_Cue"));
	if (ATTACKSOUNDCUE1.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE1.Object);
	}
	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE2(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_54_Cue.Low_Furious_VOB_P2_54_Cue"));
	if (ATTACKSOUNDCUE2.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE2.Object);
	}
	// ���� ���� ����
	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACKSOUNDCUE3(TEXT("/Game/Voices_Of_Battle_FURIOUS_P1/Cue/03_Low_Pitch/Low_Furious_VOB_P2_2_Cue.Low_Furious_VOB_P2_2_Cue"));
	if (ATTACKSOUNDCUE3.Succeeded())
	{
		AttackSoundCue.Add(ATTACKSOUNDCUE3.Object);
	}

}
void AEDR_Enemy_AxeSkeleton::BeginPlay()
{
	Super::BeginPlay();

	// ���� ����� ĳ���� �տ� ���� ����
	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon = (GetWorld()->SpawnActor<AEDR_Enemy_SWeaponAxe>(FVector::ZeroVector, FRotator::ZeroRotator));
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180�� ȸ��
		//CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // �����̵�


	}
}