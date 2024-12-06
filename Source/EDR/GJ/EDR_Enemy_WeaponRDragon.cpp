// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_WeaponRDragon.h"

AEDR_Enemy_WeaponRDragon::AEDR_Enemy_WeaponRDragon()
{
	// �޽� ������
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));

	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
}
