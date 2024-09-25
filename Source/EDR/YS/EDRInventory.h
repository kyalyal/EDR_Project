// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EDRInventoryStruct.h"
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

	UFUNCTION()
	void InteractInventory();

	UFUNCTION()
	bool PickUpItem(class AEDRInteractItem* Item);


	//최대 갯수가 다 찼는지
	TTuple<bool,int32> IsKeyFull(int32 Key);

	//아이템 추가
	UFUNCTION()
	void ItemAdded(int32 SlotKey, FEDR_InventoryStruct SlotValue);

	//드롭
	UFUNCTION()
	void ItemDropped(FEDR_InventoryStruct ItemDropped);

	//제거
	UFUNCTION()
	void ItemRemove(int32 Key);


	
private:

	UPROPERTY(EditAnywhere)
	TMap<int32, FEDR_InventoryStruct> Inventory;

	//최대 사이즈
	int32 MaxSize;

	//중복 허용
	bool AllowDuplicates;





	//레퍼런스---------------------------------------

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


	TArray<AActor*> HitActor;
	TArray<AActor*> IgnoreActor;

		
};
