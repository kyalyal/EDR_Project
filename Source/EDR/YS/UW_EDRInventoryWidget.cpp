// Fill out your copyright notice in the Description page of Project Settings.



#include "UW_EDRInventoryWidget.h"
#include "EDRInventory.h"
#include "Kismet/GameplayStatics.h"
#include "UW_EDRInvGrid.h"
#include "EDRCharacter.h"




void UUW_EDRInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//플레이어 인벤토리 동기화
	Player = Cast< AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	WidgetInventory = Player->GetEDRInventory()->GetInventory();
	WidgetSize = Player->GetEDRInventory()->GetMaxSize();
	DuplicatesAllowed = Player->GetEDRInventory()->GetAllowDuplicates();

	


}

void UUW_EDRInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();


	//바인드

	UMG_EDR_Inv_Grid->Inv_ItemAdded.BindUFunction(this, FName("InvItemAddedFunc"));
	UMG_EDR_Inv_Grid->Inv_ItemRemoved.BindUFunction(this, FName("InvItemRemovedFunc"));


	//백그라운드 설정 실행
	UMG_EDR_Inv_Grid->CreateGridBackGround();


	TArray<int32> Keys;
	WidgetInventory.GetKeys(Keys);
	for (int32 Key : Keys)
	{
		FEDR_InventoryStruct Item = *WidgetInventory.Find(Key);

		if (Item.DataAsset->Image == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("DataAsset->Image = nullptr"));
		}

		UMG_EDR_Inv_Grid->AddItemToGrid(Key, Item);
		
	}
}

void UUW_EDRInventoryWidget::InvItemRemovedFunc(int32 Key)
{
	WidgetInventory.Remove(Key);
	//ItemRemoved.Execute(Key); //나중에 체크 풀어줌
}

void UUW_EDRInventoryWidget::InvItemAddedFunc(int32 Key, FEDR_InventoryStruct Value)
{
	WidgetInventory.Add({ Key,Value });

	if (ItemAdded.IsBound())
	{
		ItemAdded.Execute(Key, Value);
	}
}


