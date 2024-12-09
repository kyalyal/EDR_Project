// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "EDR_Enemy_Skeleton.generated.h"

/*
 * 
 */
UCLASS()
class EDR_API AEDR_Enemy_Skeleton : public AMyCharacter
{
	GENERATED_BODY()
public:
	AEDR_Enemy_Skeleton();
protected:
	//virtual void BeginPlay()override;
	virtual void BeginPlay() override;
};
