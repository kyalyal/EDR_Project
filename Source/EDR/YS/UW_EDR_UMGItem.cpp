

#include "UW_EDR_UMGItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"



UUW_EDR_UMGItem::UUW_EDR_UMGItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UUW_EDR_UMGItem::NativeConstruct()
{
	Super::NativeConstruct();

	
}

FReply UUW_EDR_UMGItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (ItemStructure.Quantity != 1)
		{
			SplitStack.Execute(this);
		}
	}
	else
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

		}
	}

	return FReply::Handled();
}

void UUW_EDR_UMGItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	


	SetVisibility(ESlateVisibility::HitTestInvisible);

	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>(this);

	//오브젝트 담기
	DragOperation->Payload = this;

	//드레그 드롭할 때 비쥬얼
	DragOperation->DefaultDragVisual = this;

	OutOperation = DragOperation;

	

}

bool UUW_EDR_UMGItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UUW_EDR_UMGItem* L_OldItem = Cast<UUW_EDR_UMGItem>(InOperation->Payload);

	
	if (IsValid(L_OldItem))
	{
		//현재 아이템과 새로 들어온 드롭된 아이템이 다른지 비교
		if (L_OldItem != this)
		{
			//데이터 에셋이 같은지 비교
			if (L_OldItem->GetDataAsset() == GetDataAsset())
			{
				
				if (GetMaxStack() == GetQuantity())
				{
					L_OldItem->SetVisibility(ESlateVisibility::Visible);

					return false;
				}
				else
				{
					//곂친거 끼리 더했을때 최대소지량보다 크면
					if ((L_OldItem->GetQuantity() + GetQuantity()) > GetMaxStack())
					{

						L_OldItem->UpdateQuantity((L_OldItem->GetQuantity() + GetQuantity()) - GetMaxStack());

						UpdateQuantity(GetMaxStack());
					}
					else
					{

						UpdateQuantity((L_OldItem->GetQuantity() + GetQuantity()));

						L_OldItem->RemoveItem();

						return true;
					}
				}
			}
			else
			{
				L_OldItem->SetVisibility(ESlateVisibility::Visible);
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}


	return false;
}



void UUW_EDR_UMGItem::InitItemInfo(int32 KeyRef, FEDR_InventoryStruct Value)
{


	Key = KeyRef;
	ItemStructure = Value;

	if (!IMG_Visual)
	{
		UE_LOG(LogTemp, Error, TEXT("IMG_Visual is nullptr"));
		return;
	}

	if (!ItemCount)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemCount is nullptr"));
		return;
	}

	// ItemStructure.DataAsset이 유효한지 확인
	if (ItemStructure.DataAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemStructure.DataAsset is nullptr"));
		return;
	}

	// DataAsset->Image가 유효한지 확인
	if (ItemStructure.DataAsset->Image == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemStructure.DataAsset->Image is nullptr"));
		return;
	}

	
	IMG_Visual->SetBrushFromTexture(ItemStructure.DataAsset->Image);

	ItemCount->SetText(FText::AsNumber(ItemStructure.Quantity));


}

void UUW_EDR_UMGItem::UpdateQuantity(int32 NewQuantity)
{
	ItemStructure.Quantity = NewQuantity;

	ItemCount->SetText(FText::AsNumber(NewQuantity));

	SetVisibility(ESlateVisibility::Visible);

	
	QuantityModified.Execute(Key, ItemStructure);
}

void UUW_EDR_UMGItem::RemoveItem()
{
	ItemRemoved.Execute(Key);

	RemoveFromParent();
}

int32 UUW_EDR_UMGItem::GetMaxStack()
{
	return ItemStructure.DataAsset->MaxStack;
}

UDA_EDRItem* UUW_EDR_UMGItem::GetDataAsset()
{
	return ItemStructure.DataAsset;
}

int32 UUW_EDR_UMGItem::GetQuantity()
{
	return ItemStructure.Quantity;
}
