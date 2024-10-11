// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDRInvSlot.h"
#include "UW_EDR_UMGItem.h"
#include "Blueprint/DragDropOperation.h"



bool UUW_EDRInvSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	



	UUW_EDR_UMGItem* Item = Cast<UUW_EDR_UMGItem>(InOperation->Payload);

	if (IsValid(Item))
	{
		if (Item->Key != Key)
		{
			ItemPlaced.Execute(Key, Item->ItemStructure);

			Item->RemoveItem();

			return true;

		}
		else
		{
			Item->SetVisibility(ESlateVisibility::Visible);

			return true;
		}
	}


	return false;
}