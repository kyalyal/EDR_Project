// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_SWeaponAxe.h"

AEDR_Enemy_SWeaponAxe::AEDR_Enemy_SWeaponAxe()
{
	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/Axe/Axe.Axe"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	//SphereRadius = 20.0f; // 원하는 스피어의 반지름
	Damage = 10.0f;
	////TracePoint
	//AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
	//AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

	//AttackPointStart->SetupAttachment(WeaponMesh);
	//AttackPointEnd->SetupAttachment(WeaponMesh);

	//AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	//AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	AttackPointStart->SetRelativeLocation(FVector(10.0f, 0.f, -5.0f));
	AttackPointEnd->SetRelativeLocation(FVector(-30.f, 0.f, -5.0f));
}