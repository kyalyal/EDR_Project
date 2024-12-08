// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Skeleton.h"
#include "EDR_Enemy_SWeapon.h"

AEDR_Enemy_Skeleton::AEDR_Enemy_Skeleton()
{
	IsBoss = false;
	DetectRange = 1500.0f;

	IsFightStarting = true;


	// hp 재정의
	hp = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 300.0f;
	AttackRadius = 150.0f;

	SkillDamage = 20.0f;
	SkillRange = 300.0f;
	SkillRadius = 150.0f;

	// 속도
	TargetSpeed = 0.0f;
	Acceleration = 1.0f;
	Deceleration = 200.0f;
	RotationSpeed = 5.0f;
	StopDistance = 100.0f;
	MaxWalkSpeed = 250.0f;
	// 캐릭터 메시랑 캡슐콜리전
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// 메시 받아오기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/SKnight_modular/Skeleton_Knight_01/mesh/SK_SKnigh_01_full.SK_SKnigh_01_full"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}


	// 공격
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE1(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Attack/Anim_Sword_1H_Attack_Combo1_Montage.Anim_Sword_1H_Attack_Combo1_Montage"));
	if (ATTACK_MONTAGE1.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE1.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE2(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Attack/Anim_Sword_1H_Attack_Slash2_Montage.Anim_Sword_1H_Attack_Slash2_Montage"));
	{
		AttackMontage.Add(ATTACK_MONTAGE2.Object);
	}

	// 사망 애니메이션 몽타주 저장
	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/SkeletonEnemy/Deaths_Hit_Head_Spinning_Montage.Deaths_Hit_Head_Spinning_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}


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

}
void AEDR_Enemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();

	// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon.Add(GetWorld()->SpawnActor<AEDR_Enemy_SWeapon>(FVector::ZeroVector, FRotator::ZeroRotator));
	if (nullptr != CurrentWeapon[0])
	{
		CurrentWeapon[0]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180도 회전
		//CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // 무기이동


	}
}