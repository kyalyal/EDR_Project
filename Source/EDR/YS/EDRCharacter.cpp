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
	static ConstructorHelpers::FObjectFinder<UAnimMontage>AM_BackStep(TEXT("/Game/YS/Animation/GKnight/Roll/GKnight_DodgeBackward_Root_Montage.GKnight_DodgeBackward_Root_Montage"));
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


	//컨트롤 모드
	CurrentControlMode = EControlMode::None;

	TargetLockCameraInterpSpeed = 1.f;

	ignores.Add(this);


	//테스트용 스피어 컴포넌트
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOLLISION"));
	SphereCollision->SetSphereRadius(500);
	SphereCollision->SetupAttachment(RootComponent);
	

	//공격 초기화
	ComboAttackMontage = InitAttackMontage;

	
}

void AEDRCharacter::BeginPlay()
{

	Super::BeginPlay();

	EDRAnimInstance = GetMesh()->GetAnimInstance();

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
			TargetLock(TargetLockActor, DeltaSecond);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("EDRCharacter Tick - NoneLockTarget."));
		}

		break;

	}
	
	
	
	if (this == UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		FString CurrentState = StaticEnum<EControlState>()->GetNameStringByValue(static_cast<int64>(GetControlState()));
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Purple, FString::Printf(TEXT("Player Current Control State : %s"), *CurrentState));

		FString CurrentMode = StaticEnum<EControlMode>()->GetNameStringByValue(static_cast<int64>(GetControlMode()));
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Purple, FString::Printf(TEXT("Player Current Control Mode : %s"), *CurrentMode));
	}

	
	
}

void AEDRCharacter::PlayerDeath()
{
	if (OnDeath.IsBound())
	{
		OnDeath.Broadcast();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("EDRCharacter::PlayerDeath() - IsBound Fail."));
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

		InputX = MovementVector.X;
		InputY = MovementVector.Y;
	}

}

void AEDRCharacter::MoveCompleted()
{
	InputX = 0.f;
	InputY = 0.f;
}

void AEDRCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

	}
}

void AEDRCharacter::Jump()
{
	if (CurrentControlState == EControlState::None)
	{
		Super::Jump();
	}
}


bool AEDRCharacter::GetIsInput()
{
	if (!(!InputY && !InputX))
	{
		InputY = 0.f;
		InputX = 0.f;
		return true;
	}
	else
	{
		InputY = 0.f;
		InputX = 0.f;
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
		break;

	case EControlState::Attack:
		break;

	case EControlState::Rolling:
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
		if (GetIsInput())
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
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player Attack Success"));

	if (!bIsAttack && CurrentControlState == EControlState::None)
	{
		SetIsAttack(true);

		
		EDRAnimInstance->Montage_Play(ComboAttackMontage);
	}

}

void AEDRCharacter::TargetLock(AActor* TargetActor, float DeltaTime)
{
	FRotator TargetLookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	

	//카메라 고정
	FRotator LookCameraRotation = UKismetMathLibrary::RInterpTo(GetControlRotation(), TargetLookRotation, DeltaTime, TargetLockCameraInterpSpeed);
	GetController()->SetControlRotation(LookCameraRotation);


	//액터 고정
	SetActorRotation(TargetLookRotation);

}

void AEDRCharacter::CameraLockTrace()
{
	bool Trace;
	FHitResult OutHit;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * 1000.f);


	switch (CurrentControlMode)
	{
	case EControlMode::None:



		Trace = UKismetSystemLibrary::SphereTraceSingle
		(
			GetWorld(),
			StartLocation,
			EndLocation,
			CollisionRaius,
			ETraceTypeQuery::TraceTypeQuery3,
			false,
			ignores, EDrawDebugTrace::ForDuration,
			OutHit,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			3.f

		);

		if (Trace && Cast<ACharacter>(OutHit.GetActor()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerLockTraceTarget : %s"), *OutHit.GetActor()->GetName()));

			TargetLockActor = OutHit.GetActor();
			SetControlMode(EControlMode::LockMode);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("PlayerLockTraceTarget - NotFind"));
		}

		break;

	case EControlMode::LockMode:

		SetControlMode(EControlMode::None);

		break;

	};
	
}

void AEDRCharacter::ResetState()
{
	ComboAttackMontage = InitAttackMontage;

	bIsRolling = false;
	bIsAttack = false;

	SetControlState(EControlState::None);
}



