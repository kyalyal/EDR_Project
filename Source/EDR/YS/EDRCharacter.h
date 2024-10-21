// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EDRCharacterStruct.h"
#include "EDRInventoryStruct.h"
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


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathDelegateDynamic);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryButton;

public:

	//모듈형 캐릭터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Body_Chest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Body_Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Body_Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Body_Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Body_Feet;


	//파츠
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Bracers_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Bracers_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Cloaks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Gloves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Robes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Shoulders;


	//Armor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Helms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Pants;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Boots;


public:

	AEDRCharacter();

	virtual void Tick(float DeltaSecond) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	class UEDRInventory* EDRInventory;

	
	UPROPERTY(BlueprintAssignable, VisibleAnywhere ,BlueprintCallable, Category = "Player")
	FDeathDelegateDynamic OnDeath;

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


	UEDRInventory* GetEDRInventory() { return EDRInventory; }


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
	bool GetIsKeyInput(bool StopInput);

	UFUNCTION(BlueprintPure, category = "Player")
	UAnimMontage* GetSitDownMontage() { return SitDownMontage; }

	UUserWidget* GetCurrentGetItemTextWidget() { return CurrentGetItemTextWidget; }


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


	UFUNCTION(BlueprintCallable, category = "Player")
	void SetInvincible(bool NewInvincible);


protected:

	//고유 함수---------------------------------------------------

	void Interaction();

	void Rolling();

	void Attack();

	void TargetLock(AActor* TargetActor, float DeltaTime);

	int8 TargetChange();


	void CameraLockTrace();
	TArray<FHitResult> LockTraceHitResult;
	TArray<TObjectPtr<ACharacter>> LockTargetList;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> LockTraceIgnores;

	UPROPERTY(EditAnywhere)
	float CollisionRaius;

	//초과시 Target 변경
	float MaxChangeTargetValue;
	int32 CurrentTargetActorIdx;



	// 인벤토리 시스템
	void ShowInventory();



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

	//사망
	UPROPERTY()
	bool bIsDead;


	//무적
	bool bIsInvincible;


	UPROPERTY()
	TObjectPtr<class AEDRWeaponBase> CurrentWeapon;

	
	//몽타주
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> ComboAttackMontage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> InitAttackMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> BackStepMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> SitDownMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> SitUpMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "Player")
	TObjectPtr<UAnimMontage> DeathMontage;



	// 타겟 카메라 (EditAnywhere는 테스트시에만 사용)
	UPROPERTY(EditAnywhere, category= "Player")
	TObjectPtr<AActor>TargetLockActor;

	float TargetLockCameraInterpSpeed;

	

	//테스트용 컴포넌트
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> SphereCollision;

	//테스트용 컴포넌트
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> CheckCollision;

	//아이템 먹기전 위젯
	TSubclassOf<UUserWidget> GetItemWidgetTextClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentGetItemTextWidget;



	



	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
private:

	//키 입력

	float KeyInputX;
	float KeyInputY;

	float MouseInputX;
	float MouseInputY;

	float AbsX;
	float AbsY;



	//저장 초기화

	UFUNCTION(BlueprintCallable)
	void ResetState();


	//게임 인스턴스
	UPROPERTY()
	TObjectPtr<class UEDRGameInstance> EDR_GameInstance;

};

