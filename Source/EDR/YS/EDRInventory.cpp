// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRInventory.h"
#include "Kismet/GameplayStatics.h"
#include "EDRInteractItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EDRInventory.h"
#include "Blueprint/UserWidget.h"
#include "UW_EDRInventoryWidget.h"


UEDRInventory::UEDRInventory()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryWidgetClass(TEXT("/Game/YS/Blueprint/UMG/UMG_EDRInventory.UMG_EDRInventory_C"));
	if (InventoryWidgetClass.Succeeded())
	{
		UMG_InventoryClass = InventoryWidgetClass.Class;
	}

	

	MaxSize = 3;

	AllowDuplicates = true;
}


void UEDRInventory::BeginPlay()
{
	Super::BeginPlay();


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EDRInventory Component Use Success"));

	MyPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	IgnoreActor.Add(GetOwner());
	
	
}



void UEDRInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*TArray<TPair<int32, int32>> Items = GetAllInventoryItems();

	for (const TPair<int32, int32>& Item : Items)
	{
		int32 ItemID = Item.Key;
		int32 Quantity = Item.Value;

		FString ItemInfo = FString::Printf(TEXT("아이템 ID: %d, 수량: %d"), ItemID, Quantity);
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Yellow, ItemInfo);
	}*/

}

void UEDRInventory::InteractInventory()
{

	//UI가 켜져있나??
	if (IsValid(As_UMG_Inventory))
	{

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("UMG Close"));

		MyPlayerController->SetShowMouseCursor(false);
		MyPlayerController->SetInputMode(GameOnlyMode);

		//제거
		As_UMG_Inventory->RemoveFromParent();

		As_UMG_Inventory = nullptr;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("UMG Open"));

		As_UMG_Inventory = CreateWidget<UUserWidget>(GetWorld(), UMG_InventoryClass);
		if (As_UMG_Inventory != nullptr)
		{
			//위젯 화면에 뿌려주기
			As_UMG_Inventory->AddToViewport();

			InventoryWidgetScript = Cast<UUW_EDRInventoryWidget>(As_UMG_Inventory);
			if (IsValid(InventoryWidgetScript))
			{
				//델리게이트 바인딩
				InventoryWidgetScript->ItemAdded.BindUFunction(this, FName("ItemAdded"));
				InventoryWidgetScript->ItemDropped.BindUFunction(this, FName("ItemDropped"));
				InventoryWidgetScript->ItemRemoved.BindUFunction(this, FName("ItemRemove"));
			}

		}

		
		MyPlayerController->SetShowMouseCursor(true);
		MyPlayerController->SetInputMode(GameAndUIMode);
		
	}


	
	

}

bool UEDRInventory::PickUpItem(AEDRInteractItem* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("EDRInventory::PickUpItem - PickUp"));



	int32 L_QuantityIn;
	UDA_EDRItem* L_DataAssetIn;

	L_QuantityIn = Item->InventoryStructure.Quantity;
	L_DataAssetIn = Item->InventoryStructure.DataAsset;


	int32 L_Index = 0;
	
	//같은 아이템이 있는지 없는지
	bool bL_FoundSameItem =false;

	//남은 슬롯의 수
	int32 L_Remaining = 0;

	//빈슬롯 발견시 체크할 용도
	int32 L_EmptyKey = -1;



	//아이템 갯수가 있고, 인벤토리를 끝까지 돌지않은 상황에서
	while (0 < L_QuantityIn && L_Index < MaxSize)
	{
		//슬롯이 가득 찼을 때
		if (IsKeyFull(L_Index).Get<0>())
		{
			//같은 아이템이 가득 차있다
			if ((Inventory.Find(L_Index)->DataAsset) == L_DataAssetIn)
			{
				bL_FoundSameItem = true;
			}
		}
		//남았을 때
		else
		{
			L_Remaining = IsKeyFull(L_Index).Get<1>();

			//현재 슬롯에 아이템이 없는 빈 슬롯
			if (L_Remaining == -1)
			{
				if (L_EmptyKey == -1)
				{
					//빈 슬롯을 발견한 후 가장빠른 인덱스를 넣어줌
					L_EmptyKey = L_Index;
				}
			}
			//아이템이 있다면
			else
			{
				//같은 아이템인지 확인
				if ((Inventory.Find(L_Index)->DataAsset) == L_DataAssetIn)
				{
					FEDR_InventoryStruct AddInventoryStruct;
					AddInventoryStruct.DataAsset = L_DataAssetIn;
					AddInventoryStruct.Quantity = Inventory.Find(L_Index)->Quantity + UKismetMathLibrary::ClampInt64(L_QuantityIn, 0, L_Remaining);

					//아이템 갯수 계산해서 넣어주기
					Inventory.Add(L_Index, AddInventoryStruct );
				
					
					//뺏을때 음수값으로 넘어가지 않도록 제한을 걸어주기
					L_QuantityIn = UKismetMathLibrary::ClampInt64(L_QuantityIn - L_Remaining, 0, L_QuantityIn - L_Remaining);
				}
			}
		}


		//다음 슬롯으로 이동
		++L_Index;
	}
	

	//넣어야 할 수량이 없다면
	if (L_QuantityIn == 0)
	{
		return true;
	}
	//넣어야 될 수량이 있다면
	else
	{
		
		//중복이 허용되지않고 해당 슬롯에 같은 아이템이 있을 경우
		if (!AllowDuplicates && bL_FoundSameItem)
		{
			//아이템의 수량 바꾸기 ( 포기 )
			Item->ModifyQuantity(L_QuantityIn);
			
			return false;
		}
		else
		{
			//L_EmptyKey에 인덱스가 들어있는 경우 (가장 빠른 순서)
			if (L_EmptyKey != -1)
			{

				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("%d"), L_EmptyKey));

				//아이템 넣어주기
				FEDR_InventoryStruct AddInventoryStruct;
				AddInventoryStruct.DataAsset = L_DataAssetIn;
				AddInventoryStruct.Quantity = L_QuantityIn;

				Inventory.Add(L_EmptyKey, AddInventoryStruct);

				return true;
			}
			else
			{
				return false;
			}
		}

	}

}



TTuple<bool, int32> UEDRInventory::IsKeyFull(int32 Key)
{

	//슬롯에 아이템이 있는지
	if (Inventory.Find(Key))
	{
		//최대 갯수랑 넣어줄 갯수랑 차이를 구함
		int32 CalQuantity = Inventory.Find(Key)->DataAsset->MaxStack - Inventory.Find(Key)->Quantity;

	

		//가득 찼는지 안찼는지 확인
		if ((CalQuantity == 0) && (Inventory.Find(Key)->DataAsset->MaxStack != 0))
		{
			return { true,CalQuantity };
		}
		else
		{
			return { false,CalQuantity };
		}
	}
	//아직 이 슬롯에 아이템이 없음
	else
	{
		return { false,-1 };
	}



}




void UEDRInventory::ItemAdded(int32 SlotKey, FEDR_InventoryStruct SlotValue)
{
	Inventory.Add(SlotKey, SlotValue);
}



void UEDRInventory::ItemDropped(FEDR_InventoryStruct ItemDropped)
{

	FVector TargetLocation = (GetOwner()->GetActorForwardVector() * 300) + GetOwner()->GetActorLocation();
	FHitResult OutHit;



	bool bIsHit = UKismetSystemLibrary::BoxTraceSingleByProfile(
		GetWorld(),
		TargetLocation,
		TargetLocation - FVector(0.f, 0.f, 500.f),
		FVector(15.f, 15.f, 15.f),
		FRotator(0.f, 0.f, 0.f),
		TEXT("OverlapAllDynamic"),
		false,
		IgnoreActor,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f

		);



	if (bIsHit)
	{
		FActorSpawnParameters SpawnParams;
		AEDRInteractItem* DropItem = GetWorld()->SpawnActor<AEDRInteractItem>(AEDRInteractItem::StaticClass(), OutHit.ImpactPoint, FRotator(0.f, 0.f, 0.f), SpawnParams);

		if (DropItem)
		{
			DropItem->InventoryStructure = ItemDropped;
		}
	}


}

void UEDRInventory::ItemRemove(int32 Key)
{
	Inventory.Remove(Key);
}

TArray<TPair<int32, int32>> UEDRInventory::GetAllInventoryItems() const
{
	TArray<TPair<int32, int32>> ItemsWithIDAndQuantity;

	// Inventory 맵을 순회하면서 각 슬롯의 아이템 ID와 수량을 가져옴
	for (const TPair<int32, FEDR_InventoryStruct>& Slot : Inventory)
	{
		UDA_EDRItem* ItemData = Slot.Value.DataAsset;  // 아이템 데이터

		if (IsValid(ItemData))
		{
			int32 ItemID = ItemData->ItemID;           // 고유 ID (int32 타입)
			int32 Quantity = Slot.Value.Quantity;      // 해당 슬롯에 있는 아이템 수량

			// 고유 ID(ItemID)와 수량(Quantity)를 한 쌍으로 배열에 추가
			ItemsWithIDAndQuantity.Add(TPair<int32, int32>(ItemID, Quantity));
		}
	}

	return ItemsWithIDAndQuantity;
}
