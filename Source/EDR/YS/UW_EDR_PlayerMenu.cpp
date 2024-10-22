// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EDR_PlayerMenu.h"
#include "Components/Button.h"
#include "EDRCharacter.h"
#include "Kismet/GameplayStatics.h"

void UUW_EDR_PlayerMenu::NativeOnInitialized()
{
	PlayerCharacter = Cast<AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));

	Super::NativeOnInitialized();
	if (InventoryMenuButton != nullptr)
	{
		InventoryMenuButton->OnClicked.AddDynamic(PlayerCharacter, &AEDRCharacter::ShowInventory);
	}


}
