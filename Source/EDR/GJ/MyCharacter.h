// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "EDR_Enemy_Weapon.h"
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

	// ���� ��Ʈ �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> HitSoundCue;
	// ���� �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* AttackSoundCue;  
	// ��ų �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SkillSoundCue;   
	// ��� �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSoundCue;   

protected:



	UPROPERTY(VisibleAnywhere, Category = Weapone)
	// ����
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon;




	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// �޴� ������ ó�� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BluePrintReadWrite)
	float hp = 100.0f;

	// ���� ĳ������ ���� ������ ���� ����
	UPROPERTY(BluePrintReadWrite)
	float AttackDamage;
	// ���� ĳ������ ��ų ������ ���� ����
	UPROPERTY(BluePrintReadWrite)
	float SkillDamage;

	// ���� ��Ÿ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;


	// ��ų ��Ÿ�
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRadius;

	// ���� �ִϸ��̼�
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UAnimMontage>> AttackMontage;

	//  ��ų �ִϸ��̼�
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> SkillMontage;

	// ��� �ִϸ��̼�
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathMontage;

	// ���� ����
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FightStartMontage;



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

	void FightStart();


	// ���� �Լ�
	virtual void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	// ������ ���� ó��
	void AttackStep();

	// ���� ��Ʈ üũ
	void AttackCheck();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);



private:
	int32 RandomValue;
};