// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_EDR_Get_Item_Info.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UUW_EDR_Get_Item_Info : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void SetItemInfo(FText NewName, UTexture2D* NewImage);



	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image;

};
