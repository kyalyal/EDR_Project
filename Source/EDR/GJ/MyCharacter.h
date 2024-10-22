// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
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
	virtual void PossessedBy(AController* NewController)override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* AttackSoundCue;  // 공격 시 재생할 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SkillSoundCue;   // 스킬 시 재생할 사운드

protected:
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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float SkillRadius;


	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UAnimMontage>> AttackMontage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> DeathMontage;

	// 애니메이션 재생을 위해 공격중인지 확인
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


	// 애니메이션 인스턴트 객체
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


	// 공격 함수
	virtual void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	void AttackCheck();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);



private:

	int32 RandomValue;
};