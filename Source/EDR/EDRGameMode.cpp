// Copyright Epic Games, Inc. All Rights Reserved.

#include "EDRGameMode.h"
#include "EDRCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEDRGameMode::AEDRGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
