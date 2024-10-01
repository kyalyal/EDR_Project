// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EDRInventoryStruct.h"
#include "UW_EDR_UMGItem.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UUW_EDR_UMGItem : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void InitItemInfo(int32 KeyRef, FEDR_InventoryStruct Value);

	UPROPERTY()
	int32 Key;

	UPROPERTY()
	FEDR_InventoryStruct ItemStructure;




	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	class UImage* IMG_Visual;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ItemName;
};
