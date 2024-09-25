// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EDRCharacterStruct.h"
#include "EDRInventory.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EDR_API UEDRInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEDRInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void InteractInventory();

	void PickUpItem(AEDRInteractItem* Item);

	
private:

	UPROPERTY()
	TMap<int32, FEDR_InventoryStruct> Inventory;

	//최대 사이즈
	uint32 MaxSize;

	//중복 허용
	bool AllowDuplicates;



public:

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> UMG_InventoryClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> As_UMG_Inventory;


private:



	UPROPERTY()
	APlayerController* MyPlayerController;

	FInputModeGameAndUI GameAndUIMode;
	FInputModeGameOnly GameOnlyMode;

		
};
