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


}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ĳ���� ȸ�� �ε巴��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
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
	PlayAnimMontage(DeathMontage, 1.0f);
	Death = true;
	// ��ų ���� ���
	if (DeathSoundCue != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(
			DeathSoundCue,
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			false,
			1.0f,  // Volume multiplier
			0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
		);
	}
}
void AMyCharacter::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}

void AMyCharacter::FightStart()
{
	PlayAnimMontage(FightStartMontage, 1.0f);
}

void AMyCharacter::Attack()
{

	// ����, ��ų Ȯ��
	RandomValue = FMath::RandRange(0, 100);

	if (Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is Death true"));
		return;
	}

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

	// 20%Ȯ���� ��ų �ߵ�
	if (RandomValue <= 20)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Skill!!!!!!!!!!!!!!!!!"));
		PlayAnimMontage(SkillMontage, 1.3f);

		// ��ų ���� ���
		if (SkillSoundCue != nullptr)
		{
			UGameplayStatics::SpawnSoundAttached(
				SkillSoundCue,
				GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				EAttachLocation::KeepRelativeOffset,
				false,
				1.0f,  // Volume multiplier
				0.5f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
			);
		}
	}
	else
	{
		int aRandom = FMath::RandRange(0, 100);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack!@!@!@!@!@"));
		if (aRandom < 30 && aRandom >= 0)
		{
			if (AttackMontage.IsValidIndex(0))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("asdfasjkdfhslkfjhaslkjh"));
				PlayAnimMontage(AttackMontage[0], 1.0f);
			}
		}
		else if (aRandom < 60 && aRandom > 30)
		{
			if (AttackMontage.IsValidIndex(1))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("asdfasjkdfhslkfjhaslkjh"));
				PlayAnimMontage(AttackMontage[1], 1.0f);
			}
		}
		else
		{
			if (AttackMontage.IsValidIndex(2))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("asdfasjkdfhslkfjhaslkjh"));
				PlayAnimMontage(AttackMontage[2], 1.0f);
			}
		}

		// ���� ���� ���
		if (AttackSoundCue != nullptr)
		{
			UGameplayStatics::SpawnSoundAttached(
				AttackSoundCue,
				GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				EAttachLocation::KeepRelativeOffset,
				false,
				1.0f,  // Volume multiplier
				0.5f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
			);
		}
	}

	IsAttacking = true;

	// ���� ���� �̺�Ʈ �ߺ� ��� ����: ������ ���ε��� �̺�Ʈ�� ������ ����
	EDRAnim->OnAttackHitCheck.RemoveAll(this);

	// ���� ���� �̺�Ʈ ���ε�
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	// �ִϸ��̼� ���� �� ���� �� ó��
	EDRAnim->OnMontageEnded.RemoveAll(this);  // �ߺ� ���ε� ����
	EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

// ���� �ִϸ��̼� ���� ó�� �Լ�
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking)
	{
		return;
	}

	IsAttacking = false;

	// ���� ���� �� �̺�Ʈ ����
	if (EDRAnim != nullptr)
	{
		EDRAnim->OnAttackHitCheck.RemoveAll(this);  // �ߺ� ȣ�� ����
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack Animation Ended"));

	OnAttackEnd.Broadcast();
}
//  ���� ����
void AMyCharacter::AttackCheck()
{
	// ��ų ����
	if (RandomValue <= 30)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SkillCheck~~~~~~~~~~~~~~~~~~"));
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		// ��ü�� �߽��� ĳ���� �� �տ� ����
		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector ForwardLocation = StartLocation + GetActorForwardVector() * SkillRange;

		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult, // ������ �浹�� Ž���� ��� ���� ������ ���� ����ü
			StartLocation, // Ž�� ���� ��ġ
			ForwardLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2, // ���� �浹 ������ ����� Ʈ���̽� ä��
			FCollisionShape::MakeSphere(SkillRadius), // Ž�� �����  ����
			Params);

		// ����� ��� ������ ���� ����
#if ENABLE_DRAW_DEBUG
		FVector TraceVec = GetActorForwardVector() * SkillRange;
		FVector Center = StartLocation + TraceVec * 0.5f;  // ��ü�� �߽��� ����
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 1.0f;

		// ����� ���
		DrawDebugSphere(GetWorld(),
			ForwardLocation,
			SkillRadius,
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
				// ��Ʈ ���� ���
				if (HitSoundCue[1] != nullptr)
				{
					UGameplayStatics::SpawnSoundAttached(
						HitSoundCue[1],
						GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						EAttachLocation::KeepRelativeOffset,
						false,
						2.5f,  // Volume multiplier
						0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
					);
				}
				// ������ ���� ����
				FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
			}
		}
	}
	// ��������
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AttackCheck~~~~~~~~~~~~~~~~~~"));
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
				// ��Ʈ ���� ���
				if (HitSoundCue[0] != nullptr)
				{
					UGameplayStatics::SpawnSoundAttached(
						HitSoundCue[0],
						GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						EAttachLocation::KeepRelativeOffset,
						false,
						2.5f,  // Volume multiplier
						0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
					);
				}
				// ������ ���� ����
				FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
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