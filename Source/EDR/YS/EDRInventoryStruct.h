// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DA_EDRItem.h"
#include "EDRInventoryStruct.generated.h"


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
class EDR_API EDRInventoryStruct
{
public:
	EDRInventoryStruct();
	~EDRInventoryStruct();
};


USTRUCT(BlueprintType)
struct FEDRClothingList : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	EClothCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	TObjectPtr<UDataTable> Accessories;

};

USTRUCT(BlueprintType)
struct FEDRClothing : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerBracers_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerBracers_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerCloacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerGloves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerRobes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
	USkeletalMesh* PlayerShoulders;

};


USTRUCT(BlueprintType)
struct FEDR_InventoryStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDA_EDRItem* DataAsset;

};