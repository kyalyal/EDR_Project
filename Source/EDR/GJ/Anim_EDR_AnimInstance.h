// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_EDR_AnimInstance.generated.h"
// 공격이 맞았는지 확인하는 델리게이트
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

	// 몽타주 실행 함수
	void PlayAttackMontage();

	void PlayDeathMontage();
	

	// 공격이 맞았는지 확인하는 델리게이트
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	FOnAttackHitCheckDelegate OnAttackHitCheck2;

	FOnAttackHitCheckDelegate OnAttackHitCheck3;

	// 데미지 타이밍 끝을 체크 하는 델리게이트
	FOnAttackEndDelegate OnAttackEnd;

	// 공격이 맞았는지 확인하는 델리게이트
	FOnAttackStepDelegate OnAttackStep;

private:
	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	// 애니메이션 몽타주 저장할 변수
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//UAnimMontage* AttackMontage;

	//UPROPERTY()
	//TObjectPtr<class UAnimMontage> DeathMontage;


	// 공격 히트 타이밍을 노티파이로 설정
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_AttackHitCheck2();
	UFUNCTION()
	void AnimNotify_AttackHitCheck3();
	UFUNCTION()
	void AnimNotify_AttackEnd();


	//// 공격 애니메이션 재생시 스텝이 있을경우
	//UFUNCTION()
	//void AnimNotify_AttackStep();
};
