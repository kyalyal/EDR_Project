// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EDRInventoryStruct.h"
#include "UW_EDRInvGrid.generated.h"



DECLARE_DELEGATE_TwoParams(FInvItemAdded, int32, FEDR_InventoryStruct);
DECLARE_DELEGATE_OneParam(FInvItemDropped, FEDR_InventoryStruct);
DECLARE_DELEGATE_OneParam(FInvItemRemoved, int32);
/**
 * 
 */
UCLASS()
class EDR_API UUW_EDRInvGrid : public UUserWidget
{
	GENERATED_BODY()
	

public:


	UUW_EDRInvGrid(const FObjectInitializer& ObjectInitializer);

	//델리게이트
	FInvItemAdded Inv_ItemAdded;
	FInvItemDropped Inv_ItemDropped;
	FInvItemRemoved Inv_ItemRemoved;



	virtual void NativeConstruct() override;



	//Mecro;
	TTuple<int32,int32> DetermainGridLocation(int32 ChildCount, int32 MaxPerRow);



	UFUNCTION()
	void CreateGridBackGround();

	UFUNCTION()
	void AddItemToGrid(int32 Key, FEDR_InventoryStruct Value);

	UFUNCTION()
	void ItemStackSplit(class UUW_EDR_UMGItem* ItemSplit);

	UFUNCTION()
	void ItemRemoved(int32 Key);

	UFUNCTION()
	void ItemQuantityModified(int32 Key, FEDR_InventoryStruct Value);



	UPROPERTY()
	TSubclassOf<class UUW_EDRInvSlot> WidgetSlotClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> WidgetItemClass;


	UPROPERTY()
	int32 GridSize;

	UPROPERTY()
	int32 MaxRow;

	UPROPERTY()
	bool bDuplicatesAllowed;

	UPROPERTY()
	TMap<int32, class UUW_EDR_UMGItem*> GridInventory;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	class UGridPanel* EDRGridPanel;

};
