// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_SAxe.h"

AEDR_Enemy_SAxe::AEDR_Enemy_SAxe()
{
	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/SKnight_modular/Skeleton_Knight_04/mesh/weapon/SK_weapon_04.SK_weapon_04"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Damage = 10.0f;


	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 15.f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 140.0f));
}