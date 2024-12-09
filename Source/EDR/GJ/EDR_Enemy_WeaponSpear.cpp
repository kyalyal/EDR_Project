// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_WeaponSpear.h"

AEDR_Enemy_WeaponSpear::AEDR_Enemy_WeaponSpear()
{
	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Logaz_Spear.SM_Logaz_Spear"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, -20.0f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f,-80.0f));
}