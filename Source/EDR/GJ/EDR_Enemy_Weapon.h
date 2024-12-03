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


	//������ �ֱ�----------------------------------

	UFUNCTION()
	void OverlapActor(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AddIgnoreActor(AActor* IgnoreActor);

	void StopAttack();
	void StartAttack();

	void TraceAttack();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//����ƽ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	//������ �ݸ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class UBoxComponent> AttackCollision;

	//Ʈ���̽� ��������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class USceneComponent> AttackPointStart;

	//Ʈ���̽� �� ����Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	TObjectPtr<class USceneComponent> AttackPointEnd;

	//������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Weapon")
	float Damage;
	TArray<TObjectPtr<AActor>> IgnoreActors;

	bool bAttacking;
public:
	AActor* WeaponOwner;
	AActor* Player;
};
