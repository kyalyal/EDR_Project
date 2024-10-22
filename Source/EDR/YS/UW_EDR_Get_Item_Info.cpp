// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDR_Get_Item_Info.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UUW_EDR_Get_Item_Info::SetItemInfo(FText NewName, UTexture2D* NewImage)
{
	ItemName->SetText(NewName);

	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(NewImage);
	NewBrush.SetImageSize(FVector2D(512.f, 512.f));

	Image->SetBrush(NewBrush);

}
