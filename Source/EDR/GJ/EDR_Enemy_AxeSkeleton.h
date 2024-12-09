// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "EDR_Enemy_AxeSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEDR_Enemy_AxeSkeleton : public AMyCharacter
{
	GENERATED_BODY()
public:
	AEDR_Enemy_AxeSkeleton();
protected:
	virtual void BeginPlay() override;
};
