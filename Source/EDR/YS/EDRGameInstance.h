// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EDRCharacterStruct.h"
#include "EDRGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UEDRGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UEDRGameInstance();


	FEDRClothing GetEDRClothing() { return EDRClothing; }

	void SetEDRClothing(FEDRClothing NewEDRClothing) { EDRClothing = NewEDRClothing; }

private:

	FEDRClothing EDRClothing;

};
