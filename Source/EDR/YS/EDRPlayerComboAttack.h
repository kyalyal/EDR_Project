// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EDRPlayerComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UEDRPlayerComboAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> NextAttack;

	UPROPERTY(EditAnywhere)
	bool bNFIsAttack;
};
