// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_EDR_PlayerMenu.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UUW_EDR_PlayerMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;


	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	class UButton* InventoryMenuButton;

	UPROPERTY()
	class AEDRCharacter* PlayerCharacter;
	
};
