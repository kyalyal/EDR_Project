// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDR_UMGItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUW_EDR_UMGItem::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UUW_EDR_UMGItem::InitItemInfo(int32 KeyRef, FEDR_InventoryStruct Value)
{


	Key = KeyRef;
	ItemStructure = Value;
	
	IMG_Visual->SetBrushFromTexture(ItemStructure.DataAsset->Image);

	ItemName->SetText(FText::AsNumber(ItemStructure.Quantity));
}
