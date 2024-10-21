// Copyright Epic Games, Inc. All Rights Reserved.

#include "EDRGameMode.h"
#include "EDRCharacter.h"
#include "EDRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AEDRGameMode::AEDRGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/YS/Blueprint/Character/YS_BP_EDRPlayerCharacter.YS_BP_EDRPlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}


	ConstructorHelpers::FClassFinder<UUserWidget>BossHPBar(TEXT("/Game/SK/UMG/HUD/UI_BossHPPlayerView.UI_BossHPPlayerView_C"));
	if (BossHPBar.Succeeded())
	{
		BossHPBarWidgetClass = BossHPBar.Class;
	}


}

void AEDRGameMode::SetFightMode(EFightMode NewFightMode)
{
	FightMode = NewFightMode;

	switch (FightMode)
	{
	case EFightMode::None:

		FightEnd();

		break;
	case EFightMode::FightMode:

		FightStart();


		break;
	default:
		break;
	}

}


void AEDRGameMode::FightStart_Implementation()
{
	//BossHP 위젯
	if (BossHPBarWidgetClass != nullptr)
	{
		CurrentBossHPBarWidget = CreateWidget<UUserWidget>(GetWorld(), BossHPBarWidgetClass);
		if (CurrentBossHPBarWidget != nullptr)
		{
			CurrentBossHPBarWidget->AddToViewport();
		}


	}
}


void AEDRGameMode::FightEnd_Implementation()
{
	if (IsValid(CurrentBossHPBarWidget))
	{
		CurrentBossHPBarWidget->RemoveFromParent();
	}
}
