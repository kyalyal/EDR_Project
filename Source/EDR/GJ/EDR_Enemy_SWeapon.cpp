// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_SWeapon.h"

AEDR_Enemy_SWeapon::AEDR_Enemy_SWeapon()
{
	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}
