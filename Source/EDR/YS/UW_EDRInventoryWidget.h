// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EDRInventoryStruct.h"
#include "UW_EDRInventoryWidget.generated.h"




DECLARE_DELEGATE_TwoParams(FItemAdded, int32, FEDR_InventoryStruct);
DECLARE_DELEGATE_OneParam(FItemDropped, FEDR_InventoryStruct);
DECLARE_DELEGATE_OneParam(FItemRemoved, int32);


/**
 * 
 */
UCLASS()
class EDR_API UUW_EDRInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	//델리게이트 부분

	FItemAdded ItemAdded;
	FItemDropped ItemDropped;
	FItemRemoved ItemRemoved;

	//델리게이트 함수

	UFUNCTION()
	void InvItemRemovedFunc(int32 Key);

	UFUNCTION()
	void InvItemAddedFunc(int32 Key, FEDR_InventoryStruct Value);



	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<class UUW_EDRInvGrid> UMG_EDR_Inv_Grid;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FEDR_InventoryStruct> WidgetInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WidgetSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DuplicatesAllowed;





	//인벤토리
	TObjectPtr<class AEDRInventory> InventoryComponent;

	//플레이어
	TObjectPtr<class AEDRCharacter> Player;
	
};
