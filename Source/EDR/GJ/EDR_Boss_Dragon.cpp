// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Dragon.h"
#include "GameFramework/CharacterMovementComponent.h"

AEDR_Boss_Dragon::AEDR_Boss_Dragon()
{

	IsBoss = true;
	// hp 재정의
	hp = 100.0f;
	AttackDamage = 40.0f;
	AttackRange = 500.0f;
	AttackRadius = 200.0f;

	SkillDamage = 40.0f;
	SkillRange = 400.0f;
	SkillRadius = 200.0f;


	// 속도
	TargetSpeed = 200.0f;
	Acceleration = 120.0f;

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

}