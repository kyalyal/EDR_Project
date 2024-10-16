// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Anim_EDR_AnimInstance.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsAttacking = false;


	static ConstructorHelpers::FObjectFinder<UAnimMontage>DEATHANIM(TEXT("/Game/GJ/Animation/Enemy_Anim/GiantEnemy/Death1_Montage.Death1_Montage"));
	if (DEATHANIM.Succeeded())
	{
		DeathMontage = DEATHANIM.Object;
	}
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

// ������ �޴� �Լ�
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Death) return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);;

	UpdateHP(-DamageAmount);

	if (hp <= 0)
	{
		hp = 0;
		IsDeath();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AMyCharacter::IsDeath()
{
	PlayAnimMontage(DeathMontage);

	Death = true;
}

// ������ �޾����� hp ������Ʈ
void AMyCharacter::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}


// ���� �ÿ� ȣ��Ǵ� �Լ�
void AMyCharacter::Attack()
{
	if (Death) return;
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
	EDRAnim->PlayAttackMontage();
	
	IsAttacking = true;
}

// ���� ���� ���� ����
void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	EDRAnim = Cast<UAnim_EDR_AnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == EDRAnim)
	{
		return;
	}

	EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

// ���� �ִϸ��̼� ��
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