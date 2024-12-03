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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Get

	UFUNCTION(BlueprintPure, category = "Weapon")
	float GetDamage() { return Damage; }

	//Set

	UFUNCTION(BlueprintCallable, category = "Weapon")
	void SetDamage(float NewDamage);
	// Sets default values for this actor's properties
	AEDR_Enemy_Weapon();


	//데미지 주기----------------------------------

	UFUNCTION()
	void OverlapActor(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AddIgnoreActor(AActor* IgnoreActor);

	void StopAttack();
	void StartAttack();

	void TraceAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//스태틱 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	//데미지 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class UBoxComponent> AttackCollision;

	//트레이스 시작포인트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class USceneComponent> AttackPointStart;

	//트레이스 끝 포인트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class USceneComponent> AttackPointEnd;

	//데미지
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	float Damage;
	TArray<TObjectPtr<AActor>> IgnoreActors;

	bool bAttacking;
public:
	AActor* WeaponOwner;
	AActor* Player;
};
