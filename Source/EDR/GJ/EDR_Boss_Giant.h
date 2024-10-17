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
	float AttackDamage;
	float hp = 20;

	virtual void IsDeath();

	virtual void Attack();

protected:

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void UpdateHP(float NewHP);
private:
};
