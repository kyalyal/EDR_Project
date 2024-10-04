// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDRInvGrid.h"
#include "UW_EDRInvSlot.h"
#include "UW_EDR_UMGItem.h"
#include "Components/GridPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UUW_EDRInvGrid::UUW_EDRInvGrid(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)  // 부모 클래스의 생성자를 호출
{
	
	static ConstructorHelpers::FClassFinder<UUW_EDRInvSlot> InventorySlotWidgetClass(TEXT("/Game/YS/Blueprint/UMG/UMG_EDR_Inv_Slot.UMG_EDR_Inv_Slot_C"));
	if (InventorySlotWidgetClass.Succeeded())
	{
		WidgetSlotClass = InventorySlotWidgetClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find UMG_EDR_Inv_Slot"));
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryItemWidgetClass(TEXT("/Game/YS/Blueprint/UMG/UMG_EDRItem.UMG_EDRItem_C"));
	if (InventoryItemWidgetClass.Succeeded())
	{
		WidgetItemClass = InventoryItemWidgetClass.Class;
	}

	// 그리드의 기본 크기 설정
	MaxRow = 10;
	GridSize = 55;
}


void UUW_EDRInvGrid::NativeConstruct()
{
	Super::NativeConstruct();


}

TTuple<int32, int32> UUW_EDRInvGrid::DetermainGridLocation(int32 ChildCount, int32 MaxPerRow)
{

	int32 CalRow = floor(((float)(ChildCount / MaxPerRow)));

	

	return { (ChildCount - (CalRow * MaxPerRow)), CalRow };
}

void UUW_EDRInvGrid::CreateGridBackGround()
{
	

	for (int32 i = 0; i < GridSize; i++)
	{
		//아이템 생성
		TObjectPtr<UUW_EDRInvSlot> ItemSlot;
		ItemSlot = CreateWidget<UUW_EDRInvSlot>(GetWorld(),WidgetSlotClass);


		//UUW_EDRInvSlot* ItemSlotRef = Cast<UUW_EDRInvSlot>(ItemSlot);

		if (ItemSlot == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ItemSlotRef is NULL. Cast to UUW_EDRInvSlot failed."));
			return;
		}

		ItemSlot->Key = i;
		ItemSlot->ItemPlaced.BindUFunction(this, FName("AddItemToGrid"));



		//아이템 그리드에 붙이기
		TTuple<int32,int32> CalGrid = DetermainGridLocation(i, MaxRow);

		EDRGridPanel->AddChildToGrid(ItemSlot, CalGrid.Get<1>(), CalGrid.Get<0>() );
	}
}

void UUW_EDRInvGrid::AddItemToGrid(int32 Key, FEDR_InventoryStruct Value)
{
	TObjectPtr<UUserWidget> Item;
	Item = CreateWidget<UUserWidget>(GetWorld(), WidgetItemClass);

	UUW_EDR_UMGItem* EDRItem = Cast<UUW_EDR_UMGItem>(Item);

	
	//델리게이트 바인드
	EDRItem->SplitStack.BindUFunction(this, FName("ItemStackSplit"));
	EDRItem->ItemRemoved.BindUFunction(this, FName("ItemRemoved"));
	EDRItem->QuantityModified.BindUFunction(this, FName("ItemQuantityModified"));


	if (Value.DataAsset->Image == nullptr)
	{

	}
	else
	{
		EDRItem->InitItemInfo(Key, Value);
	}


	//그리드 셀에 추가하기
	TTuple<int32, int32> CalGrid = DetermainGridLocation(Key, MaxRow);
	EDRGridPanel->AddChildToGrid(EDRItem, CalGrid.Get<1>(), CalGrid.Get<0>());
	


	GridInventory.Add(Key, EDRItem);

	Inv_ItemAdded.Execute(Key,Value);

}

void UUW_EDRInvGrid::ItemStackSplit(UUW_EDR_UMGItem* ItemSplit)
{

	if (bDuplicatesAllowed)
	{
		int32 L_NewQuantity;
		int32 L_OldQuantity;

		for (int32 i = 0; i < GridSize; i++)
		{
			if (GridInventory.Contains(i))
			{
				L_NewQuantity = ItemSplit->ItemStructure.Quantity / 2;

				L_OldQuantity = ItemSplit->ItemStructure.Quantity - L_NewQuantity;

				
				FEDR_InventoryStruct NewInvST;
				NewInvST.Quantity = L_NewQuantity;
				NewInvST.DataAsset = ItemSplit->ItemStructure.DataAsset;

				ItemSplit->UpdateQuantity(L_OldQuantity);

				AddItemToGrid(i, NewInvST);

				return;
			}
		}
	}
	else
	{
		return;
	}
}

void UUW_EDRInvGrid::ItemRemoved(int32 Key)
{
	GridInventory.Remove(Key);

	Inv_ItemRemoved.Execute(Key);

}

void UUW_EDRInvGrid::ItemQuantityModified(int32 Key, FEDR_InventoryStruct Value)
{
	Inv_ItemAdded.Execute(Key, Value);
}
