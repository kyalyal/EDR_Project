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

DECLARE_MULTICAST_DELEGATE(FOnFightStartEndDelegate);


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

	// ���� �̽� �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> MissSoundCue;

	// ���� �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> AttackSoundCue;

	// ��ų �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SkillSoundCue;

	// ��� �� ����� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSoundCue;

	// ���� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;
protected:

	// �ȴ� �ӵ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetSpeed = 200.f; // ��ǥ �ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 120.0f;  // ���ӵ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentSpeed = 0.0f; // ���� �ӵ�

	// �̵������� üũ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsMoving = false;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapone)
	// ����
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon;
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon2;
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon3;


	// ���� ���� �Լ�
	void StopMovement();

	void ResumeMovement();

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



	// ��ų ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
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
	// ���� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange;


	// ���� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBoss;

	// ���� ���� �ִϸ��̼� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsFightStarting = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BluePrintReadWrite)
	bool Death = false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// hp ��ȯ
	UFUNCTION(BlueprintPure, category = "Player")
	float GetHp() { return hp; }


	// hp ������Ʈ
	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void UpdateHP(float NewHP);


	// ��� �ִϸ��̼� ���
	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void IsDeath();


	// ���� ���� �ִϸ��̼� ������ �� ȣ���� ��������Ʈ
	FOnFightStartEndDelegate OnFightStartEnd;

	// ���ο� �Լ� �߰�
	UFUNCTION()
	void OnFightStartMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ���� ���۽� �ִϸ��̼� ����ϴ� �Լ�
	void FightStart();


	// ���� �Լ�
	virtual void Attack();

	// ���� �ִϸ��̼� �������� üũ
	FOnAttackEndDelegate OnAttackEnd;



	// ���� ��Ʈ üũ
	UFUNCTION()
	void AttackCheck();
	UFUNCTION()
	void AttackCheckEnd();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bCanAttackSmallMove; //���ݹ̼��̵�����. 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))FVector ExpectedAttackLocation; //���ݹ̼��̵���ǥ��.


private:

	FTimerHandle StopMovementTimerHandle;
	int32 RandomValue;
};