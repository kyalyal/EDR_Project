// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EDRCharacterStruct.h"
#include "EDRCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UENUM(BlueprintType)
enum class EControlMode : uint8
{
	None,
	LockMode
};

UENUM(BlueprintType)
enum class EControlState : uint8
{
	None,
	Attack,
	Rolling,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegate);


UCLASS(config=Game)
class AEDRCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//Interact 버튼
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RollingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LockCamera;


public:
	AEDRCharacter();

	virtual void Tick(float DeltaSecond) override;
	
	UPROPERTY(BlueprintAssignable,BlueprintCallable, Category = "Event")
	FDeathDelegate OnDeath;

	UFUNCTION()
	void PlayerDeath();

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void MoveCompleted();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void Jump() override;
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;


	//데미지 받기
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;




public:
	//Get--------------------------------------------------


	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	UFUNCTION(BlueprintPure, category = "Player")
	float GetHP() { return CharacterInfo.HP; }


	UFUNCTION(BlueprintPure, category = "Player")
	bool GetIsRolling() { return bIsRolling; }

	UFUNCTION(BlueprintPure, category = "Player")
	bool GetIsAttack() { return bIsAttack; }


	UFUNCTION(BlueprintPure, category = "Player")
	FCharacterAbility GetCharacterInfo() { return CharacterInfo; }

	UFUNCTION(BlueprintPure, category = "Player")
	EControlState GetControlState() { return CurrentControlState; }

	UFUNCTION(BlueprintPure, category = "Player")
	EControlMode GetControlMode() { return CurrentControlMode; }


	UFUNCTION(BlueprintPure, category = "Player")
	bool GetIsMoving() { if (GetVelocity().Length()) { return true; } else { return false; } }

	UFUNCTION(BlueprintPure, category = "Player")
	bool GetIsInput(bool StopInput);


	//Set--------------------------------------------------

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetHP(float NewHP);

	void UpdateHP(float NewHP);


	UFUNCTION(BlueprintCallable, category = "Player")
	void SetIsRolling(bool Roll);

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetIsAttack(bool Attack);

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetComboAttackMontage(UAnimMontage* NextAttack);

	void SetControlMode(EControlMode NewControlMode);

	void SetControlState(EControlState NewControlState);


	UFUNCTION(BlueprintCallable, category = "Player")
	void SetCharacterInfo(FCharacterAbility NewCharacterInfo);



protected:

	//고유 함수---------------------------------------------------

	void Interaction();

	void Rolling();

	void Attack();

	void TargetLock(AActor* TargetActor, float DeltaTime);

	void CameraLockTrace();
	TArray<FHitResult> LockTraceHitResult;
	TArray<AActor*> LockTraceIgnores;
	float CollisionRaius = 50.f;

	UFUNCTION(BlueprintCallable)
	void ResetState();




	//고유 변수--------------------------------------------------------


public:
	

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCharacterAbility CharacterInfo;



	//컨트롤 Edit모드는 테스트 시에만 사용
	UPROPERTY(EditAnywhere, category = "Player")
	EControlMode CurrentControlMode;


	//행동 상태
	UPROPERTY(EditAnywhere, category = "Player")
	EControlState CurrentControlState;



	// 애니메이션
	TObjectPtr<UAnimInstance> EDRAnimInstance;


	//구르기
	UPROPERTY()
	bool bIsRolling;

	//공격
	UPROPERTY()
	bool bIsAttack;

	
	//몽타주
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> InitAttackMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> BackStepMontage;



	// 타겟 카메라 (EditAnywhere는 테스트시에만 사용)
	UPROPERTY(EditAnywhere, category= "Player")
	TObjectPtr<AActor>TargetLockActor;

	float TargetLockCameraInterpSpeed;

	

	//테스트용 컴포넌트
	UPROPERTY()
	TObjectPtr<class USphereComponent> SphereCollision;

	
private:

	float InputX;
	float InputY;

};

