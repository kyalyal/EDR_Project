// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YS/EDRPlayerInterface.h"
#include "EDR_NPC_Base.generated.h"

UCLASS()
class EDR_API AEDR_NPC_Base : public AActor, public IEDRPlayerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDR_NPC_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 인터페이스 함수 구현 (BlueprintNativeEvent로 선언)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact();

	// C++에서 직접 구현
	void Interact_Implementation();


	virtual void PlayerInteract() override;

};
