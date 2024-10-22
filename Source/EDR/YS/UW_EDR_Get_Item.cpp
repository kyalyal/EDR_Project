// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDR_Get_Item.h"
#include "UW_EDR_Get_Item_Info.h"
#include "Components/TextBlock.h"


void UUW_EDR_Get_Item::SetItemInfo(FText NewText, UTexture2D* NewImage)
{
	UI_NewItemGet->SetItemInfo(NewText, NewImage);
}


void UUW_EDR_Get_Item::NativeConstruct()
{
	Super::NativeConstruct();


}
