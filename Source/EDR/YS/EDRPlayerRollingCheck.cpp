// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerRollingCheck.h"
#include "EDRCharacter.h"

UEDRPlayerRollingCheck::UEDRPlayerRollingCheck()
{
	
}

void UEDRPlayerRollingCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	PlayerCharacter = Cast<AEDRCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("PlayerRollingCheck -> Animation Name : %s"),*Animation->GetName()));

		PlayerCharacter->SetIsRolling(bIsAnimRolling);
	}
}
