// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EDRPlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEDRPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EDR_API IEDRPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void PlayerInteract() = 0;
};
