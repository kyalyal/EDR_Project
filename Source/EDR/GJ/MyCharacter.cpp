// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Engine/World.h"
#include "Anim_EDR_AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsAttacking = false;
	// ĸ��������Ʈ�� MyCharacter�������� ����ϵ��� ��
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));
	// ���� ���� ���� �ʱ�ȭ
	AttackRange = 400.0f;
	AttackRadius = 200.0f;
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// ĳ���� ȸ�� �ε巴��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ���� �ִϸ��̼� ����
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking)
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("end play"));
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

// ��� �ִϸ��̼�
void AMyCharacter::IsDeath()
{
	// �̹� �׾������� �ִϸ��̼� ���� ����
	if (Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Death true"));
		return;
	}

	// �ִϸ��̼� ��Ÿ�� ����
	PlayAnimMontage(DeathMontage);
	Death = true;
}
void AMyCharacter::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}

void AMyCharacter::Attack()
{

	// Death�� true �ϰ�� ���� �ִϸ��̼� ���� 
	
	if (Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is Death true"));
		return;
	}

	// �������� ��� �ִϸ��̼� ����
	if (IsAttacking)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is attacking true"));
		return;
	}

	EDRAnim = Cast<UAnim_EDR_AnimInstance>(GetMesh()->GetAnimInstance());
	
	if (nullptr == EDRAnim)
	{
		return;
	}

	// �ִϸ��̼� ��Ÿ�� ����
	PlayAnimMontage(AttackMontage, 0.5f);
	IsAttacking = true;
	// ���� ����
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
	// ���� ����
	EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

//  ���� ����
void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	// ��ü�� �߽��� ĳ���� �� �տ� ����
	FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FVector ForwardLocation = StartLocation + GetActorForwardVector() * AttackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, // ������ �浹�� Ž���� ��� ���� ������ ���� ����ü
		StartLocation, // Ž�� ���� ��ġ
		ForwardLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // ���� �浹 ������ ����� Ʈ���̽� ä��
		FCollisionShape::MakeSphere(AttackRadius), // Ž�� �����  ����
		Params);

	// ����� ��� ������ ���� ����
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = StartLocation + TraceVec * 0.5f;  // ��ü�� �߽��� ����
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	// ����� ���
	DrawDebugSphere(GetWorld(),
		ForwardLocation,
		AttackRadius,
		12, // ���׸�Ʈ ��, �� �������� �ε巯����
		DrawColor,
		false,
		DebugLifeTime);

#endif
	// ���� ������
	if (bResult)
	{
		if (HitResult.GetActor() != nullptr)
		{
			// ������ ���� ����
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}
}


// ������ �޴� �Լ�
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Death)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}


	// ���� ������ ��ŭ hp ����
	UpdateHP(-DamageAmount);


	// hp�� 0�� �Ǿ������ ��� �Լ� ȣ��
	if (this->hp <= 0)
	{
		this->hp = 0;
		IsDeath();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
