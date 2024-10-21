// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EDRGameMode.generated.h"

UENUM(BlueprintType)
enum class EFightMode : uint8
{
	None,
	FightMode
};


UCLASS(minimalapi)
class AEDRGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEDRGameMode();

	//Set
	UFUNCTION()
	void SetFightMode(EFightMode NewFightMode);


	//Get
	UFUNCTION()
	EFightMode GetFightMode() { return FightMode; }



public:

	UFUNCTION()
	void FightStart();

	UFUNCTION()
	void FightEnd();


	
private:

	EFightMode FightMode;
};



