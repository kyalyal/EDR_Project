// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EDRCharacter.h"
#include "EDRPlayerCharacter.generated.h"

/**
 * 
 */



UCLASS()
class EDR_API AEDRPlayerCharacter : public AEDRCharacter
{
	GENERATED_BODY()

public:

	AEDRPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSecond) override;

	//데미지 받기
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	



public:
	//Get

	UFUNCTION(BlueprintPure, category = "Player")
	float GetHP() { return HP; }

	//Set

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetHP(float NewHP);

	
	void UpdateHP(float NewHP);



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HP;

	
};



