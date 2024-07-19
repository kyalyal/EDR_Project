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


	void StartAttack();
	void StopAttack();

protected:


	//스태틱 메시
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	//콜리전
	UPROPERTY(EditAnywhere, category = "Weapon")
	TObjectPtr<class UBoxComponent> AttackCollision;


	//공격 트레이스
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, category = "Weapon")
	TObjectPtr<USceneComponent> LineTraceStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<USceneComponent> LineTraceEnd;


	FTimerHandle AttackTimerHandle;

	float AttackDelayTime = 0.001f;


	//데미지
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, category = "Weapon")
	float Damage;

	
};
