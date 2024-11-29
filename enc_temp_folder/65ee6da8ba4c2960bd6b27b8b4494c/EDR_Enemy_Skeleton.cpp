// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Skeleton.h"
#include "EDR_Enemy_SWeapon.h"

AEDR_Enemy_Skeleton::AEDR_Enemy_Skeleton()
{
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 300.0f;
	AttackRadius = 150.0f;

	SkillDamage = 20.0f;
	SkillRange = 300.0f;
	SkillRadius = 150.0f;


	//GetCharacterMovement()->MaxWalkSpeed = 200.0f;

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
}
void AEDR_Enemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();

	// 게임 실행시 캐릭터 손에 무기 장착
	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_SWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		//CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180도 회전
		//CurrentWeapon->SetActorRelativeLocation(FVector(0.0f, 0.0f, -10.0f)); // 무기이동


	}
}