// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EDRWeaponBase.generated.h"

UCLASS()
class EDR_API AEDRWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEDRWeaponBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	//Get

	UFUNCTION(BlueprintPure, category = "Weapon")
	float GetDamage() { return Damage; }


	//Set

	UFUNCTION(BlueprintCallable, category = "Weapon")
	void SetDamage(float NewDamage);

public:

	//데미지 주기
	void ApplyDamage();

	FDamageEvent DamageEvent();


protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, category = "Weapon")
	TObjectPtr<class UBoxComponent> AttackCollision;


	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, category = "Weapon")
	float Damage;

	
};
