// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_BWeapon.h"

AEDR_Enemy_BWeapon::AEDR_Enemy_BWeapon()
{	

    // �⺻ Į
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/GreatSword/SM_MERGED_StaticMeshActor_16.SM_MERGED_StaticMeshActor_16"));
    if (SK_WEAPON.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
    }
    WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// ���� ũ�� Ű���
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));
	//TracePoint
	//AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
	//AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

	//AttackPointStart->SetupAttachment(WeaponMesh);
	//AttackPointEnd->SetupAttachment(WeaponMesh);

	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 115.0f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 5.0f));
	//SphereRadius = 40.0f; // ���ϴ� ���Ǿ��� ������

	Damage = 30.0f;
}