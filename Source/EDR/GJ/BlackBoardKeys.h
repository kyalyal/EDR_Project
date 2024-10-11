// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
/**
 * 
 */
namespace bb_keys
{
	TCHAR const* const target_location = TEXT("TargetLocation");
	TCHAR const* const can_see_player = TEXT("CanSeePlayer");
	TCHAR const* const player_is_in_melee_range = TEXT("IsPlayerInMeRange");

}
class EDR_API BlackBoardKeys
{
public:
	BlackBoardKeys();
	~BlackBoardKeys();
};
