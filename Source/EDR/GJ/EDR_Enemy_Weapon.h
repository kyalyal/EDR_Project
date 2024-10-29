// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDR_Enemy_Weapon.generated.h"

UCLASS()
class EDR_API AEDR_Enemy_Weapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEDR_Enemy_Weapon();

	// 무기의 스태틱 메시를 담을 변수
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Weapon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
