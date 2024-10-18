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

// 공격 애니메이션 종료
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

// 사망 애니메이션
void AMyCharacter::IsDeath()
{
	// 이미 죽어있으면 애니메이션 실행 안함
	if (Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Death true"));
		return;
	}
	
	// 애니메이션 몽타주 실행
	PlayAnimMontage(DeathMontage);
	Death = true;
}
void AMyCharacter::UpdateHP(float NewHp)
{

}

void AMyCharacter::Attack()
{

	// Death가 true 일경우 공격 애니메이션 정지 
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is alsdhkjfgalskjdfhalkudfhasl,djfhasdlkhfjbn true"));
	if (Death)
	{
		return;
	}

	// 공격중일 경우 애니메이션 정지
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

	// 애니메이션 몽타주 실행
	PlayAnimMontage(AttackMontage, 1.0f);
	IsAttacking = true;

}

// 데미지 받는 함수
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Death) return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);;


	// 입은 데미지 만큼 hp 차감
	UpdateHP(-DamageAmount);


	// hp가 0이 되었을경우 사망 함수 호출
	if (this->hp <= 0)
	{
		this->hp = 0;
		IsDeath();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
