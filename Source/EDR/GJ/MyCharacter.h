// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "EDR_Enemy_Weapon.h"
#include "MyCharacter.generated.h"


// 공격 애니메이션이 끝났는지 확인
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


	// 공격 히트 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> HitSoundCue;

	// 공격 미스 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> MissSoundCue;

	// 공격 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<TObjectPtr<class USoundCue>> AttackSoundCue;

	// 스킬 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SkillSoundCue;

	// 사망 시 재생할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSoundCue;

	// 공격 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;
protected:

	// 걷는 속도 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetSpeed = 200.f; // 목표 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 120.0f;  // 가속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CurrentSpeed = 0.0f; // 현재 속도

	// 이동중인지 체크
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsMoving = false;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapone)
	// 무기
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon;
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon2;
	TObjectPtr<class AEDR_Enemy_Weapon> CurrentWeapon3;


	// 정지 관련 함수
	void StopMovement();

	void ResumeMovement();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 받는 데미지 처리 함수
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BluePrintReadWrite)
	float hp = 100.0f;

	// 보스 캐릭터의 공격 데미지 설정 변수
	UPROPERTY(BluePrintReadWrite)
	float AttackDamage;
	// 보스 캐릭터의 스킬 데미지 설정 변수
	UPROPERTY(BluePrintReadWrite)
	float SkillDamage;



	// 스킬 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRadius;

	// 공격 애니메이션
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UAnimMontage>> AttackMontage;

	//  스킬 애니메이션
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> SkillMontage;

	// 사망 애니메이션
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathMontage;

	// 전투 시작
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> FightStartMontage;





	// 애니메이션 재생을 위해 공격중인지 확인
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


	// 애니메이션 인스턴트 객체
	UPROPERTY()
	class UAnim_EDR_AnimInstance* EDRAnim;

public:
	// 보스 인지 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange;


	// 보스 인지 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBoss;

	// 전투 시작 애니메이션 확인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsFightStarting = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BluePrintReadWrite)
	bool Death = false;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	// hp 반환
	UFUNCTION(BlueprintPure, category = "Player")
	float GetHp() { return hp; }


	// hp 업데이트
	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void UpdateHP(float NewHP);


	// 사망 애니메이션 재생
	UFUNCTION(BlueprintCallable, category = "Player")
	virtual void IsDeath();


	// 전투 시작 애니메이션 끝났을 때 호출할 델리게이트
	FOnFightStartEndDelegate OnFightStartEnd;

	// 새로운 함수 추가
	UFUNCTION()
	void OnFightStartMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 전투 시작시 애니메이션 재생하는 함수
	void FightStart();


	// 공격 함수
	virtual void Attack();

	// 공격 애니메이션 끝났는지 체크
	FOnAttackEndDelegate OnAttackEnd;



	// 공격 히트 체크
	UFUNCTION()
	void AttackCheck();
	UFUNCTION()
	void AttackCheckEnd();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bCanAttackSmallMove; //공격미세이동여부. 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))FVector ExpectedAttackLocation; //공격미세이동목표값.


private:

	FTimerHandle StopMovementTimerHandle;
	int32 RandomValue;
};