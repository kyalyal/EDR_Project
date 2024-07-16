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
	BossMode
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

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

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
	FCharacterAbility GetCharacterInfo() { return CharacterInfo; }


	//Set--------------------------------------------------

	UFUNCTION(BlueprintCallable, category = "Player")
	void SetHP(float NewHP);

	void UpdateHP(float NewHP);


	UFUNCTION(BlueprintCallable, category = "Player")
	void SetIsRolling(bool Roll);

	void SetControlMode(EControlMode NewControlMode);


	UFUNCTION(BlueprintCallable, category = "Player")
	void SetCharacterInfo(FCharacterAbility NewCharacterInfo);

protected:

	//고유 함수---------------------------------------------------

	void Interaction();

	void Rolling();

	void TargetLock(AActor* TargetActor, float DeltaTime);


	//고유 변수--------------------------------------------------------


public:
	

protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCharacterAbility CharacterInfo;



	//컨트롤 Edit모드는 테스트 시에만 사용
	UPROPERTY(EditAnywhere, category = "Player")
	EControlMode CurrentControlMode;


	// 애니메이션
	TObjectPtr<UAnimInstance> EDRAnimInstance;


	//구르기
	bool bIsRolling;

	
	//EditAnywhere는 테스트시에만 사용
	UPROPERTY(EditAnywhere, category= "Player")
	TObjectPtr<AActor>TargetLockActor;

	float TargetLockCameraInterpSpeed;
	

	//테스트용 컴포넌트
	UPROPERTY()
	TObjectPtr<class USphereComponent> SphereCollision;
};

