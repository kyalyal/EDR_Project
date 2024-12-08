// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRWeapon_Axe.h"

AEDRWeapon_Axe::AEDRWeapon_Axe() : AEDRWeaponBase()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Weapon(TEXT("/Game/LJY/Axe/StaticMesh/SM_MERGED_Axe.SM_MERGED_Axe"));
    if (SM_Weapon.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SM_Weapon.Object);
        WeaponMesh->AddLocalRotation(FRotator(0.f,90.f,0.f));
        WeaponMesh->AddLocalOffset(FVector(0.f, 0.f, 20.f));
    }

    //애니메이션
    ConstructorHelpers::FObjectFinder<UAnimMontage>WA(TEXT("/Game/YS/Animation/ROG_Male/ROG_Axe/EDR_Anim_SAS_Combo4_1_Montage.EDR_Anim_SAS_Combo4_1_Montage"));
    if (WA.Succeeded())
    {
        WeaponAttackAnim = WA.Object;
    }
}
