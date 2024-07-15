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
#include "EDRPlayerInterface.h"

//DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEDRCharacter

AEDRCharacter::AEDRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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
	CurrentControlMode = EControlMode::BossMode;

	TargetLockCameraInterpSpeed = 1.f;


	//테스트용 스피어 컴포넌트
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOLLISION"));
	SphereCollision->SetSphereRadius(500);
	SphereCollision->SetupAttachment(RootComponent);
	
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

	case EControlMode::BossMode:

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

float AEDRCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UpdateHP(-DamageAmount);


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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEDRCharacter::Rolling);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEDRCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEDRCharacter::Look);
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
	}
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
		
	case EControlMode::BossMode:

		//카메라 회전에 따른 움직임 제어
		GetCharacterMovement()->bUseControllerDesiredRotation = false;

		break;
	}
}

void AEDRCharacter::SetCharacterInfo(FCharacterAbility NewCharacterInfo)
{
	CharacterInfo = NewCharacterInfo;
}


void AEDRCharacter::Rolling()
{

	if (!IsValid(EDRAnimInstance)) return;

	bIsRolling = true;
	
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
