// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDRPlayerInterface.h"
#include "EDRCharacterStruct.h"
#include "EDRInteractItem.generated.h"

UCLASS()
class EDR_API AEDRInteractItem : public AActor, public IEDRPlayerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDRInteractItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	virtual void PlayerInteract() override;


	TObjectPtr<class AEDRCharacter> PlayerCharacter;

	UFUNCTION()
	void PlayerDie();


	UFUNCTION()
	void ModifyQuantity(int32 NewQuantity);


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Item")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category="Item")
	FEDR_InventoryStruct InventoryStructure;

};
