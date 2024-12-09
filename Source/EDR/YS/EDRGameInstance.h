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



	//Get
	//(BlueprintPure)
	FEDRClothing GetEDRClothing() { return EDRClothing; }

	FVector GetPlayerStartLocation() { return PlayerStartLocation; }

	FRotator GetPlayerStartRotation() { return PlayerStartRotation; }

	FRotator GetControllerStartRotation() { return ControllerStartRotation; }
	


	//Set
	void SetEDRClothing(FEDRClothing NewEDRClothing) { EDRClothing = NewEDRClothing; }

	void SetPlayerStartLocation(FVector NewLocation) { PlayerStartLocation = NewLocation; }

	void SetPlayerStartRotation(FRotator NewRotation) { PlayerStartRotation = NewRotation; }

	void SetControllerStartRotation(FRotator NewRotation) { ControllerStartRotation = NewRotation; }



public:



	UPROPERTY(BlueprintReadWrite)
	FEDRClothing EDRClothing;

private:

	UPROPERTY()
	FVector PlayerStartLocation;

	UPROPERTY()
	FRotator PlayerStartRotation;

	UPROPERTY()
	FRotator ControllerStartRotation;



};
