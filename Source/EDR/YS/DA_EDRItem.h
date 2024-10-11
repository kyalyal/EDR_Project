// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_EDRItem.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UDA_EDRItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere)
	int32 ItemID;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	int32 MaxStack;


	UPROPERTY(EditAnywhere)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere)
	USkeletalMesh* SK_Mesh;
};
