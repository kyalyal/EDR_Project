// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_BWeapon.h"

AEDR_Enemy_BWeapon::AEDR_Enemy_BWeapon()
{	

	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Vanter_Sword.SM_Vanter_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}