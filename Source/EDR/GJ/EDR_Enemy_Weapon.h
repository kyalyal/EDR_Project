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
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Weapon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 무기 메시


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
