// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRGameInstance.h"

UEDRGameInstance::UEDRGameInstance()
{
	//테스트용
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TESTSK(TEXT("/Game/ROG_Modular_Armor/Accessories_Robes/ROG_CodeSpartan/ROG_Male/Robes/SkeletalMeshes/SK_Chest2_Light_T2.SK_Chest2_Light_T2"));
	if (TESTSK.Succeeded())
	{
		EDRClothing.PlayerRobes = TESTSK.Object;
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TESTSK2(TEXT("/Game/ROG_Modular_Armor/Accessories_Robes/ROG_CodeSpartan/ROG_Male/Cloaks/SkeletalMeshes/SK_Cloak0_T1_Red.SK_Cloak0_T1_Red"));
	if (TESTSK.Succeeded())
	{
		EDRClothing.PlayerCloacks = TESTSK2.Object;
	}
}
