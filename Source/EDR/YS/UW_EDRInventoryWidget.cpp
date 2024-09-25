// Fill out your copyright notice in the Description page of Project Settings.



#include "UW_EDRInventoryWidget.h"
#include "EDRInventory.h"
#include "Kismet/GameplayStatics.h"
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


