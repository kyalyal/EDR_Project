// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_BWeapon.h"

AEDR_Enemy_BWeapon::AEDR_Enemy_BWeapon()
{	

    // �⺻ Į
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Vanter_Sword.SM_Vanter_Sword"));
    if (SK_WEAPON.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
    }
    WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// ���� ũ�� Ű���
	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
	//TracePoint
	//AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
	//AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

	//AttackPointStart->SetupAttachment(WeaponMesh);
	//AttackPointEnd->SetupAttachment(WeaponMesh);

	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, -65.0f));
	//SphereRadius = 40.0f; // ���ϴ� ���Ǿ��� ������

	Damage = 1.0f;
}