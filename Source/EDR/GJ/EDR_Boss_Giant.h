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
	// ���� ĳ������ ���� ������ ���� ����
	float AttackDamage;
	// ���� ĳ���� ü��
	float hp = 100;

	// ���� ĳ���Ϳ��� ������ �ؼ� ����� ��� ó�� �Լ�
	virtual void IsDeath() override;

	// ���� �Լ�
	virtual void Attack() override;

protected:

	// �޴� ������ ó�� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	// hp ������Ʈ �Լ�
	virtual void UpdateHP(float NewHP);
private:
};
