// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_EDR_AnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
/**
 * 
 */
UCLASS()
class EDR_API UAnim_EDR_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAnim_EDR_AnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// ��Ÿ�� ���� �Լ�
	void PlayAttackMontage();

	void PlayDeathMontage();
	
	FOnAttackHitCheckDelegate OnAttackHitCheck;
private:
	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	// �ִϸ��̼� ��Ÿ�� ������ ����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeathMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();
};
