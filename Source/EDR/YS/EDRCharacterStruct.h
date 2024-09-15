// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EDRCharacterStruct.generated.h"


UENUM(BlueprintType)
enum class EClothCategory : uint8
{
	Bracers,
	Cloaks,
	Gloves,
	Robes,
	Shoulders

};


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
struct FCharacterAbility : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

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

USTRUCT(BlueprintType)
struct FEDRClothing : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EClothCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TObjectPtr<UDataTable> Accessories;

};
