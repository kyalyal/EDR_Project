// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EDRInventoryStruct.h"
#include "UW_EDRInvSlot.generated.h"


DECLARE_DELEGATE_TwoParams(FItemPlaced, int32, FEDR_InventoryStruct);

/**
 * 
 */
UCLASS()
class EDR_API UUW_EDRInvSlot : public UUserWidget
{
	GENERATED_BODY()

public :



	//델리게이트
	FItemPlaced ItemPlaced;


	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


	UPROPERTY(BlueprintReadOnly)
	int32 Key;
};
