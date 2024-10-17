// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "GameFramework/Character.h"
#include "EDR_Boss_Giant.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEDR_Boss_Giant : public AMyCharacter
{
	GENERATED_BODY()
public:
	AEDR_Boss_Giant();

	UPROPERTY(BlueprintReadWrite)
	// 보스 캐릭터의 공격 데미지 설정 변수
	float AttackDamage;
	// 보스 캐릭터 체력
	float hp = 100;

	// 마이 캐릭터에서 재정의 해서 사용할 사망 처리 함수
	virtual void IsDeath() override;

	// 공격 함수
	virtual void Attack() override;

protected:

	// 받는 데미지 처리 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	// hp 업데이트 함수
	virtual void UpdateHP(float NewHP);
private:
};
