// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRInventory.h"
#include "Kismet/GameplayStatics.h"
#include "EDRInteractItem.h"
#include "Blueprint/UserWidget.h"


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

	
	
	
}



void UEDRInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
			As_UMG_Inventory->AddToViewport();
		}

		
		MyPlayerController->SetShowMouseCursor(true);
		MyPlayerController->SetInputMode(GameAndUIMode);
		
	}


	
	

}

void UEDRInventory::PickUpItem(AEDRInteractItem* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("EDRInventory::PickUpItem - PickUp"));
}
