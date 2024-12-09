// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_SWeaponAxe.h"

AEDR_Enemy_SWeaponAxe::AEDR_Enemy_SWeaponAxe()
{
	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/FlexibleCombatSystem/DemoRoom/Meshes/SM_Axe.SM_Axes"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
	}
	WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	Damage = 15.0f;


	AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 15.f));
	AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 140.0f));
}