// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Giant.h"
#include "Anim_EDR_AnimInstance.h"

AEDR_Boss_Giant::AEDR_Boss_Giant()
{

}
void AEDR_Boss_Giant::IsDeath()
{
	if (Super::Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Death true"));
		return;
	}
	EDRAnim->PlayDeathMontage();

	Super::Death = true;
}
// ���� �ÿ� ȣ��Ǵ� �Լ�
void AEDR_Boss_Giant::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is alsdhkjfgalskjdfhalkudfhasl,djfhasdlkhfjbn true"));
	if (Super::Death) 
	{ 
		return;
	}
	if (Super::IsAttacking)
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

	Super::IsAttacking = true;
}

// ������ �޴� �Լ�
float AEDR_Boss_Giant::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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


// ������ �޾����� hp ������Ʈ
void AEDR_Boss_Giant::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}