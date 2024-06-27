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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSecond) override;


//Get
public:


	UFUNCTION(BlueprintPure, category = "Player")
	float GetHP() { return HP; }

//Set
public:

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetHP(float NewHP);


private:

	UPROPERTY()
	float HP;
	
};
