// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EDRPlayerRollingCheck.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UEDRPlayerRollingCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UEDRPlayerRollingCheck();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY()
	TObjectPtr<class AEDRCharacter> PlayerCharacter;

	UPROPERTY(EditAnywhere)
	bool bIsAnimRolling;
};
