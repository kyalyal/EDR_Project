// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Anim_EDR_AnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsAttacking = false;

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
	IsAttacking = false;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("end play"));
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
void AMyCharacter::UpdateHP(float NewHp)
{

}

void AMyCharacter::Attack()
{

	// Death�� true �ϰ�� ���� �ִϸ��̼� ���� 
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is alsdhkjfgalskjdfhalkudfhasl,djfhasdlkhfjbn true"));
	if (Death)
	{
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
	PlayAnimMontage(AttackMontage, 1.0f);
	IsAttacking = true;

}

// ������ �޴� �Լ�
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Death) return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);;


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
