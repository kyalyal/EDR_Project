// Copyright Epic Games, Inc. All Rights Reserved.

#include "EDRCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EDRPlayerInterface.h"
#include "EDRWeaponBase.h"

//DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEDRCharacter

AEDRCharacter::AEDRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	
	//Interact 키 생성
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Interact(TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact.IA_Interact"));
	if (IA_Interact.Succeeded())
	{
		InteractAction = IA_Interact.Object;
	}

	//Attack 키 생성
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Attack(TEXT("/Game/ThirdPerson/Input/Actions/IA_Attack.IA_Attack"));
	if (IA_Attack.Succeeded())
	{
		AttackAction = IA_Attack.Object;
	}

	//Rolling 키 생성
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Rolling(TEXT("/Game/ThirdPerson/Input/Actions/IA_Rolling.IA_Rolling"));
	if (IA_Attack.Succeeded())
	{
		RollingAction = IA_Rolling.Object;
	}

	//LockCamera 키 생성
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_LockCamera(TEXT("/Game/ThirdPerson/Input/Actions/IA_LockCamera.IA_LockCamera"));
	if (IA_LockCamera.Succeeded())
	{
		LockCamera = IA_LockCamera.Object;
	}

	//백스텝 몽타주 넣어주기
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AM_BackStep(TEXT("/Game/YS/Animation/ROG_Male/ROG_Roll/EDR_GKnight_DodgeBackward_Root_Montage.EDR_GKnight_DodgeBackward_Root_Montage"));
	if (AM_BackStep.Succeeded())
	{
		BackStepMontage = AM_BackStep.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage>AM_InitAttack(TEXT("/Game/YS/Animation/Sword/EDR_2H_Attack_A_Montage.EDR_2H_Attack_A_Montage"));
	if (AM_InitAttack.Succeeded())
	{
		InitAttackMontage = AM_InitAttack.Object;
	}



	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 


	//모듈형 메시

	Body_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODYARMS"));
	Body_Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODYCHEST"));
	Body_Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODYHANDS"));
	Body_Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODYLEGS"));
	Body_Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BODYFEET"));

	Body_Arms->SetupAttachment(GetMesh());
	Body_Chest->SetupAttachment(GetMesh());
	Body_Hands->SetupAttachment(GetMesh());
	Body_Legs->SetupAttachment(GetMesh());
	Body_Feet->SetupAttachment(GetMesh());


	//컨트롤 모드
	CurrentControlMode = EControlMode::None;


	//타겟 변경

	TargetLockCameraInterpSpeed = 10.f;

	CollisionRaius = 500.f;
	LockTraceIgnores.Add(this);


	
	MaxChangeTargetValue = 100.f;
	CurrentTargetActorIdx = 0;

	TEnumAsByte<EObjectTypeQuery> TargetPawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(TargetPawn);



	//테스트용 스피어 컴포넌트
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOLLISION"));
	SphereCollision->SetSphereRadius(500);
	SphereCollision->SetupAttachment(RootComponent);
	

	//공격 초기화
	bIsAttack = false;

	
	//캐릭터 info 초기화
	CharacterInfo.HP = 100.f;


}

void AEDRCharacter::BeginPlay()
{

	Super::BeginPlay();

	EDRAnimInstance = GetMesh()->GetAnimInstance();

	CameraBoom->AddLocalOffset(FVector(0.f, 0.f, 100.f));


	//공격 초기화
	ComboAttackMontage = InitAttackMontage;


	CurrentWeapon = GetWorld()->SpawnActor<AEDRWeaponBase>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,TEXT("WeaponSoket_r"));
		CurrentWeapon->AddIgnoreActor(this);
	}

}

void AEDRCharacter::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);


	switch (CurrentControlMode)
	{

	case EControlMode::None:
		break;

	case EControlMode::LockMode:

		if (IsValid(TargetLockActor))
		{
			//타겟 고정
			TargetLock(TargetLockActor, DeltaSecond);

			//타겟 바꾸기
			if (int8 NextIdx = TargetChange())
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Tick() - Change Target"));

				AbsX = 0.f;
				AbsY = 0.f;

				CurrentTargetActorIdx += NextIdx;

				if (CurrentTargetActorIdx > LockTargetList.Num()-1)
				{
					CurrentTargetActorIdx = 0;
				}
				else if (CurrentTargetActorIdx < 0)
				{
					CurrentTargetActorIdx = LockTargetList.Num() - 1;
				}

				TargetLockActor = LockTargetList[CurrentTargetActorIdx];
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Yellow, TEXT("EDRCharacter Tick - NoneLockTarget."));
		}

		break;

	}
	
	
	
	if (this == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		//상태 디버깅
		FString CurrentState = StaticEnum<EControlState>()->GetNameStringByValue(static_cast<int64>(GetControlState()));
		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Purple, FString::Printf(TEXT("Player Current Control State : %s"), *CurrentState));

		FString CurrentMode = StaticEnum<EControlMode>()->GetNameStringByValue(static_cast<int64>(GetControlMode()));
		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Purple, FString::Printf(TEXT("Player Current Control Mode : %s"), *CurrentMode));

		/*if(GetIsKeyInput(false))
		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Green, TEXT("PlayerPressMoveKey"));

		if (GetIsRolling())
		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Green, TEXT("PlayerPressRollingKey"));*/


		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Purple, FString::Printf(TEXT("LockTraceHitResult.Num() : %d"), LockTargetList.Num()));

		if (LockTargetList.Num())
		{
			for (int32 i = 0; i < LockTargetList.Num(); i++)
			{
				GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Blue, FString::Printf(TEXT("TargetActorList - %s"), *(LockTargetList[i]->GetName())));
			}
		}
		
		GEngine->AddOnScreenDebugMessage(-1, DeltaSecond, FColor::Purple, FString::Printf(TEXT("CurrentTargetActorIdx : %d"), CurrentTargetActorIdx));
	}
	
}

void AEDRCharacter::PlayerDeath()
{
	if (OnDeath.IsBound()  == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("EDRCharacter::PlayerDeath() - Broadcast Success."));
		OnDeath.Broadcast();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("EDRCharacter::PlayerDeath() - Not Find Binding Delegate Actor."));
	}

}

float AEDRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UpdateHP(-DamageAmount);

	if (CharacterInfo.HP <= 0)
	{
		PlayerDeath();
		CharacterInfo.HP = 0;
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}




//////////////////////////////////////////////////////////////////////////
// 입력

void AEDRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 입력 매핑
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEDRCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEDRCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AEDRCharacter::MoveCompleted);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEDRCharacter::Look);

		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AEDRCharacter::Interaction);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AEDRCharacter::Attack);

		//Rolling
		EnhancedInputComponent->BindAction(RollingAction, ETriggerEvent::Started, this, &AEDRCharacter::Rolling);

		//LockCamera
		EnhancedInputComponent->BindAction(LockCamera, ETriggerEvent::Started, this, &AEDRCharacter::CameraLockTrace);
	}
	else
	{

	}
}

void AEDRCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		KeyInputX = MovementVector.X;
		KeyInputY = MovementVector.Y;
	}

}

void AEDRCharacter::MoveCompleted()
{
	KeyInputX = 0.f;
	KeyInputY = 0.f;
}

void AEDRCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (CurrentControlMode != EControlMode::LockMode)
		{
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}

		MouseInputX = LookAxisVector.X;
		MouseInputY = LookAxisVector.Y;
	}
}

void AEDRCharacter::Jump()
{
	if (CurrentControlState == EControlState::None)
	{
		Super::Jump();
	}
}


bool AEDRCharacter::GetIsKeyInput(bool StopInput)
{
	if (!(!KeyInputY && !KeyInputX))
	{
		if (StopInput)
		{
			KeyInputY = 0.f;
			KeyInputX = 0.f;
		}
		return true;
	}
	else
	{
		if (StopInput)
		{
			KeyInputY = 0.f;
			KeyInputX = 0.f;
		}
		return false;
	}
}

void AEDRCharacter::SetHP(float NewHP)
{
	CharacterInfo.HP = NewHP; 
}

void AEDRCharacter::UpdateHP(float NewHP)
{
	CharacterInfo.HP += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), CharacterInfo.HP));
}

void AEDRCharacter::SetIsRolling(bool Roll)
{

	bIsRolling = Roll;


	if (bIsRolling)
	{
		SetControlState(EControlState::Rolling);
	}
	else
	{
		SetControlState(EControlState::None);
	}

}

void AEDRCharacter::SetIsAttack(bool Attack)
{
	bIsAttack = Attack;

	if (bIsAttack)
	{
		SetControlState(EControlState::Attack);
	}
	else
	{
		SetControlState(EControlState::None);
	}
}

void AEDRCharacter::SetComboAttackMontage(UAnimMontage* NextMontage)
{
	ComboAttackMontage = NextMontage;
}

void AEDRCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::None:

		//카메라 회전에 따른 움직임 제어
		GetCharacterMovement()->bUseControllerDesiredRotation = true;


		break;
		
	case EControlMode::LockMode:

		//카메라 회전에 따른 움직임 제어
		GetCharacterMovement()->bUseControllerDesiredRotation = false;


		break;
	}
}

void AEDRCharacter::SetControlState(EControlState NewControlState)
{
	CurrentControlState = NewControlState;

	switch (CurrentControlState)
	{
	case EControlState::None:

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->StopAttack();
		}

		break;

	case EControlState::Attack:

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->StartAttack();
		}

		break;

	case EControlState::Rolling:

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->StopAttack();
		}

		break;


	default:
		break;
	}
}

void AEDRCharacter::SetCharacterInfo(FCharacterAbility NewCharacterInfo)
{
	CharacterInfo = NewCharacterInfo;
}


void AEDRCharacter::Interaction()
{
	//Sphere Collision Test
	TArray<AActor*> OverlapActor;
	SphereCollision->GetOverlappingActors(OverlapActor);

	for (AActor* i : OverlapActor)
	{
		IEDRPlayerInterface* PlayerInterface = Cast<IEDRPlayerInterface>(i);
		if (PlayerInterface)
		{
			PlayerInterface->PlayerInteract();
		}
	}

}

void AEDRCharacter::Rolling()
{

	if (!IsValid(EDRAnimInstance) || GetCharacterMovement()->IsFalling()) return;

	//구르기 상태가 아니거나 백스텝 몽타주가 실행중이 아니라면
	if (!bIsRolling && !EDRAnimInstance->Montage_IsPlaying(BackStepMontage))
	{
		if (GetIsKeyInput(true))
		{
			SetIsRolling(true);

		}
		else
		{
			if (IsValid(BackStepMontage))
			{

				PlayAnimMontage(BackStepMontage);

			}
		}


	}
	
}

void AEDRCharacter::Attack()
{
	

	if (!bIsAttack && CurrentControlState == EControlState::None)
	{
		SetIsAttack(true);
		
		EDRAnimInstance->Montage_Play(ComboAttackMontage);

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Attack() - Success"));
	}

}

void AEDRCharacter::TargetLock(AActor* TargetActor, float DeltaTime)
{
	FRotator TargetLookRotationCal = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	FRotator TargetLookRotation = FRotator(0.f, TargetLookRotationCal.Yaw, TargetLookRotationCal.Roll);


	//카메라 고정
	FRotator LookCameraRotationCal = UKismetMathLibrary::RInterpTo(GetControlRotation(), TargetLookRotation, DeltaTime, TargetLockCameraInterpSpeed);
	GetController()->SetControlRotation(LookCameraRotationCal);


	//액터 고정
	SetActorRotation(TargetLookRotation);

}

int8 AEDRCharacter::TargetChange()
{
	
	if (LockTargetList.Num() >= 2)
	{
		if (UKismetMathLibrary::Abs(MouseInputX) > 0.5f)
		{
			AbsX += MouseInputX;
		}
		if (UKismetMathLibrary::Abs(MouseInputY) > 0.5f)
		{
			AbsY += MouseInputY;
		}

		if ((AbsX + AbsY) >= MaxChangeTargetValue)
		{
			return 1;
		}
		else if ((AbsX + AbsY) <= -MaxChangeTargetValue)
		{
			return -1;
		}
	}

	return 0;
}

void AEDRCharacter::CameraLockTrace()
{
	bool Trace;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * 5000.f);



	switch (CurrentControlMode)
	{
	case EControlMode::None:



		Trace = UKismetSystemLibrary::SphereTraceMultiForObjects
		(
			GetWorld(),
			StartLocation,
			EndLocation,
			CollisionRaius,
			ObjectTypes,
			false,
			LockTraceIgnores,
			EDrawDebugTrace::ForDuration,
			LockTraceHitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.f

		);

		if (Trace)
		{
			for (FHitResult TargetCharacter : LockTraceHitResult)
			{
				LockTargetList.AddUnique(Cast<ACharacter>(TargetCharacter.GetActor()));
			}


			TargetLockActor = LockTargetList[CurrentTargetActorIdx];
			SetControlMode(EControlMode::LockMode);

			
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayerLockTraceTarget - NotFind"));
		}

		break;

	// 타겟 해제
	case EControlMode::LockMode:

		SetControlMode(EControlMode::None);

		CurrentTargetActorIdx = 0;

		//초기화
		LockTargetList.Empty();

		break;

	};
	
}

void AEDRCharacter::ResetState()
{
	ComboAttackMontage = InitAttackMontage;

	bIsRolling = false;
	bIsAttack = false;

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAttack();
	}

	SetControlState(EControlState::None);
}



