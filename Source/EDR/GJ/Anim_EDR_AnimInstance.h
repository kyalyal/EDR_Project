// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_EDR_AnimInstance.generated.h"
// 공격이 맞았는지 확인하는 델리게이트
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

	// 몽타주 실행 함수
	void PlayAttackMontage();

	void PlayDeathMontage();
	

	// 공격이 맞았는지 확인하는 델리게이트
	FOnAttackHitCheckDelegate OnAttackHitCheck;
private:
	UPROPERTY(EditAnyWhere,BlueprintReadOnly,Category = Pawn, Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	// 애니메이션 몽타주 저장할 변수
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeathMontage;


	// 공격 히트 타이밍을 노티파이로 설정
	UFUNCTION()
	void AnimNotify_AttackHitCheck();
};
