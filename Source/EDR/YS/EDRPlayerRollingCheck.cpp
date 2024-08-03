// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerRollingCheck.h"
#include "EDRPlayerCharacter.h"

UEDRPlayerRollingCheck::UEDRPlayerRollingCheck()
{
	
}

void UEDRPlayerRollingCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	PlayerCharacter = Cast<AEDRCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("PlayerRollingCheck -> Animation Name : %s"),*Animation->GetName()));

		PlayerCharacter->SetIsRolling(bIsAnimRolling);
	}
}
