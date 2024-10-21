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
	UFUNCTION(BlueprintCallable)
	void SetFightMode(EFightMode NewFightMode);


	//Get
	UFUNCTION()
	EFightMode GetFightMode() { return FightMode; }




	//플레이어 메인 위젯
	TSubclassOf<UUserWidget> BossHPBarWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentBossHPBarWidget;



public:

	UFUNCTION()
	void FightStart();

	UFUNCTION()
	void FightEnd();


	

	UPROPERTY(BlueprintReadwrite)
	EFightMode FightMode;
};



