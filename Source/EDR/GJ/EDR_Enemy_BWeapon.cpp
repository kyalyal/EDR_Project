// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_BWeapon.h"

AEDR_Enemy_BWeapon::AEDR_Enemy_BWeapon()
{	

    // 기본 칼
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/GreatSword/SM_MERGED_StaticMeshActor_16.SM_MERGED_StaticMeshActor_16"));
    if (SK_WEAPON.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
    }
    WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 무기 크기 키우기
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));
	//TracePoint
	//AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
	//AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

	//AttackPointStart->SetupAttachment(WeaponMesh);
	//AttackPointEnd->SetupAttachment(WeaponMesh);

	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 115.0f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 5.0f));
	//SphereRadius = 40.0f; // 원하는 스피어의 반지름

	Damage = 30.0f;
}