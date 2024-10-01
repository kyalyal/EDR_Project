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
	// 위젯 슬롯 클래스를 찾습니다.
	static ConstructorHelpers::FClassFinder<UUserWidget> InventorySlotWidgetClass(TEXT("/Game/YS/Blueprint/UMG/UMG_EDR_Inv_Slot.UMG_EDR_Inv_Slot_C"));
	if (InventorySlotWidgetClass.Succeeded())
	{
		WidgetSlotClass = InventorySlotWidgetClass.Class;
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
	// this가 nullptr인지 확인
	if (this == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("this is nullptr in CreateGridBackGround"));
		return;
	}

	// EDRGridPanel이 nullptr인지 확인
	if (EDRGridPanel == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EDRGridPanel is nullptr. Make sure it's bound correctly in UMG."));
		return;
	}

	for (int32 i = 0; i < GridSize; i++)
	{
		TObjectPtr<UUserWidget> ItemSlot;
		ItemSlot = CreateWidget<UUserWidget>(GetWorld(),WidgetSlotClass);

		TTuple<int32,int32> CalGrid = DetermainGridLocation(i, MaxRow);

		EDRGridPanel->AddChildToGrid(ItemSlot, CalGrid.Get<1>(), CalGrid.Get<0>() );
	}
}

void UUW_EDRInvGrid::AddItemToGrid(int32 Key, FEDR_InventoryStruct Value)
{
	TObjectPtr<UUserWidget> Item;
	Item = CreateWidget<UUserWidget>(GetWorld(), WidgetItemClass);

	UUW_EDR_UMGItem* EDRItem = Cast<UUW_EDR_UMGItem>(Item);

	if (EDRItem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("EDRItem is nullptr in AddItemToGrid."));
		return;
	}

	if (Value.DataAsset == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Value.DataAsset is nullptr. Item was not properly initialized."));
		return;
	}
	
	EDRItem->InitItemInfo(Key, Value);

	//그리드 셀에 추가하기
	TTuple<int32, int32> CalGrid = DetermainGridLocation(Key, MaxRow);
	EDRGridPanel->AddChildToGrid(EDRItem, CalGrid.Get<1>(), CalGrid.Get<0>());
	


	GridInventory.Add(Key, EDRItem);

	Inv_ItemAdded.Execute(Key,Value);

}

void UUW_EDRInvGrid::ItemStackSplit(UUW_EDR_UMGItem* ItemSplit)
{
}

void UUW_EDRInvGrid::ItemRemoved(int32 Key)
{
}

void UUW_EDRInvGrid::ItemQuantityModified(int32 Key, FEDR_InventoryStruct Value)
{
	Inv_ItemAdded.Execute(Key, Value);
}
