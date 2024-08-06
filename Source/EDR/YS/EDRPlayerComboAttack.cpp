// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerComboAttack.h"
#include "Kismet/GameplayStatics.h"
#include "EDRCharacter.h"

void UEDRPlayerComboAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TObjectPtr<AEDRCharacter> Player = Cast<AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (IsValid(Player))
	{
		Player->SetIsAttack(bNFIsAttack);
		Player->SetComboAttackMontage(NextAttack);
	}
}
