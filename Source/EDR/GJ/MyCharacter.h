// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

// 공격 애니메이션이 끝났는지 확인
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class EDR_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	UPROPERTY(BlueprintReadWrite)
	float AttackDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(BluePrintReadWrite)
	float hp = 100.0f;
	UPROPERTY(BluePrintReadWrite)
	bool Death =  false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE float GetHp(){return hp;}

	UFUNCTION(BlueprintCallable, category = "Player")
	void IsDeath();

	virtual void PostInitializeComponents() override;

	// 공격 함수
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 애니메이션 재생을 위해 공격중인지 확인
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly,  Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


	// 애니메이션 인스턴트 객체
	UPROPERTY()
	class UAnim_EDR_AnimInstance* EDRAnim;
};
