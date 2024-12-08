// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRWeapon_Axe.h"

AEDRWeapon_Axe::AEDRWeapon_Axe() : AEDRWeaponBase()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Weapon(TEXT("/Game/LJY/Axe/StaticMesh/SM_MERGED_Axe.SM_MERGED_Axe"));
    if (SM_Weapon.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SM_Weapon.Object);
    }
}
