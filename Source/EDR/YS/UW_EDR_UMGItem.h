// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EDRInventoryStruct.h"
#include "UW_EDR_UMGItem.generated.h"


DECLARE_DELEGATE_TwoParams(FQuantityModified, int32, FEDR_InventoryStruct);
DECLARE_DELEGATE_OneParam(FSplitStack, UUW_EDR_UMGItem*);
DECLARE_DELEGATE_OneParam(FItemRemoved, int32);


/**
 * 
 */
UCLASS()
class EDR_API UUW_EDR_UMGItem : public UUserWidget
{
	GENERATED_BODY()

public:


	UUW_EDR_UMGItem(const FObjectInitializer& ObjectInitializer);


	//델리게이트
	FQuantityModified QuantityModified;
	FSplitStack SplitStack;
	FItemRemoved ItemRemoved;




	//함수

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;




	UFUNCTION()
	void InitItemInfo(int32 KeyRef, FEDR_InventoryStruct Value);

	UFUNCTION()
	void UpdateQuantity(int32 NewQuantity);

	void RemoveItem();

	int32 GetMaxStack();

	UDA_EDRItem* GetDataAsset();

	int32 GetQuantity();



	//변수


	UPROPERTY()
	int32 Key;

	UPROPERTY()
	FEDR_InventoryStruct ItemStructure;




	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	class UImage* IMG_Visual;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ItemCount;
};
