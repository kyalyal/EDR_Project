// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_EDR_Get_Item.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UUW_EDR_Get_Item : public UUserWidget
{
	GENERATED_BODY()
	

public:


	void SetItemInfo(FText NewText, UTexture2D* NewImage);



	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UUW_EDR_Get_Item_Info* UI_NewItemGet;


	virtual void NativeConstruct() override;

};
