// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EDRCharacterStruct.generated.h"

/**
 * 
 */
class EDR_API EDRCharacterStruct
{
public:
	EDRCharacterStruct();
	~EDRCharacterStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterAbility
{
	GENERATED_BODY()

public:

	//레벨
	UPROPERTY(BlueprintReadWrite)
	int32 Level;

	//생명력
	UPROPERTY(BlueprintReadWrite)
	float HP;

	//정신력
	UPROPERTY(BlueprintReadWrite)
	float Mental;

	//지구력
	UPROPERTY(BlueprintReadWrite)
	float Stamina;

	//근력
	UPROPERTY(BlueprintReadWrite)
	float Strength;

	//기량
	UPROPERTY(BlueprintReadWrite)
	float Skill;

	//지력
	UPROPERTY(BlueprintReadWrite)
	float Intellectual;

	//신앙
	UPROPERTY(BlueprintReadWrite)
	float Faith;

	//신비
	UPROPERTY(BlueprintReadWrite)
	float Mystery;
};
