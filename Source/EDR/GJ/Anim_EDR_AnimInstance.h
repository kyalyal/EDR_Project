// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_EDR_AnimInstance.generated.h"
// ������ �¾Ҵ��� Ȯ���ϴ� ��������Ʈ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackHitCheckDelegate, int32, x);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackHitCheck2Delegate, int32, x);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackHitCheck3Delegate, int32, x);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackStepDelegate);
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
	

	// ������ �¾Ҵ��� Ȯ���ϴ� ��������Ʈ
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	FOnAttackHitCheckDelegate OnAttackHitCheck2;

	FOnAttackHitCheckDelegate OnAttackHitCheck3;

	// ������ Ÿ�̹� ���� üũ �ϴ� ��������Ʈ
	FOnAttackEndDelegate OnAttackEnd;

	// ������ �¾Ҵ��� Ȯ���ϴ� ��������Ʈ
	FOnAttackStepDelegate OnAttackStep;

private:
	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	// �ִϸ��̼� ��Ÿ�� ������ ����
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//UAnimMontage* AttackMontage;

	//UPROPERTY()
	//TObjectPtr<class UAnimMontage> DeathMontage;


	// ���� ��Ʈ Ÿ�̹��� ��Ƽ���̷� ����
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_AttackHitCheck2();
	UFUNCTION()
	void AnimNotify_AttackHitCheck3();
	UFUNCTION()
	void AnimNotify_AttackEnd();


	//// ���� �ִϸ��̼� ����� ������ �������
	//UFUNCTION()
	//void AnimNotify_AttackStep();
};
