// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "EDR_Boss_Knight.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEDR_Boss_Knight : public AMyCharacter
{
	GENERATED_BODY()
	AEDR_Boss_Knight();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
