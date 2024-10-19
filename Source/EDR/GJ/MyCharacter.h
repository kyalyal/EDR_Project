// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

// ���� �ִϸ��̼��� �������� Ȯ��
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class EDR_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties

	AMyCharacter();
	virtual void PossessedBy(AController* NewController)override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// �޴� ������ ó�� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BluePrintReadWrite)
	float hp = 100.0f;

	// ���� ĳ������ ���� ������ ���� ����
	UPROPERTY(BluePrintReadWrite)
	float AttackDamage;


	UPROPERTY()
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY()
	TObjectPtr<class UAnimMontage> DeathMontage;

	// �ִϸ��̼� ����� ���� ���������� Ȯ��
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


	// �ִϸ��̼� �ν���Ʈ ��ü
	UPROPERTY()
	class UAnim_EDR_AnimInstance* EDRAnim;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BluePrintReadWrite)
	bool Death = false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	UFUNCTION(BlueprintPure, category = "Player")
	float GetHp() { return hp; }

	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void UpdateHP(float NewHP);

	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void IsDeath();


	// ���� �Լ�
	virtual void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	void AttackCheck();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);



private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;


};