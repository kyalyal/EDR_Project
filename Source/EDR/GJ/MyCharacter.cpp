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
	CurrentSpeed = 0.0f;
	PrimaryActorTick.bCanEverTick = true;
	IsAttacking = false;
	IsFightStarting = false;
	// ĸ��������Ʈ�� MyCharacter�������� ����ϵ��� ��
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	TargetLocation = GetActorLocation();

}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ĳ���� ȸ�� �ε巴��
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.f, 0.0f);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = GetActorLocation();
	Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	float DistanceToTarget = FVector::Dist(TargetLocation, CurrentLocation);

	if (DistanceToTarget <= StopDistance)
	{
		TargetSpeed = 0.0f;
	}
	else
	{
		TargetSpeed = MaxWalkSpeed;
	}
	
	//// �ӵ� ����
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Acceleration); // Acceleration�� ������ ������ ����
	if (CurrentSpeed <= 10.0f)
	{
		CurrentSpeed = 0.0f;
	}
	// ĳ���� �̵� ó��
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}

	Direction.Z = 0.0f;
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 2.0f);
	SetActorRotation(NewRotation);
}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



// ĳ���� ���� ���� �Լ�
void AMyCharacter::StopMovement()
{
	// �̵��� ���߱� ���� MaxWalkSpeed�� 0���� ����
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

	// 2�� �Ŀ� �̵��� �簳
	GetWorld()->GetTimerManager().SetTimer(StopMovementTimerHandle, this, &AMyCharacter::ResumeMovement, 2.0f, false);
}

void AMyCharacter::ResumeMovement()
{
	// ������ �̵� �ӵ��� ���� (��: 200���� ����)
	GetCharacterMovement()->MaxWalkSpeed = 200.0f; // ���ϴ� �ӵ��� ����
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
	AttackCheckEnd();
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




// ���� ����

void AMyCharacter::FightStart()
{
	if (IsFightStarting)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ASDFADSF"));
		return;
	}
	EDRAnim = Cast<UAnim_EDR_AnimInstance>(GetMesh()->GetAnimInstance());
	if (FightStartMontage)
	{
		PlayAnimMontage(FightStartMontage, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("FightStartMontage Played"));
	}

	if (EDRAnim != nullptr)
	{
		EDRAnim->OnMontageEnded.RemoveAll(this);  // ���� �̺�Ʈ ����
		EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnFightStartMontageEnded);
	}
}

// ���� ���� �ִϸ��̼� ���� �Ǹ� ȣ��
void AMyCharacter::OnFightStartMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// FightStart �ִϸ��̼��� ������ �� ���� ����
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("bInterrupted: %s"), bInterrupted ? TEXT("True") : TEXT("False")));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Fight Start Animation Ended"));
	TargetSpeed = 0.0f;
	CurrentSpeed = 0.0f;
	// �ִϸ��̼� ���� Ȯ��
	IsFightStarting = true;
	//StopMovement();
	OnFightStartEnd.Broadcast();
}




// ���� ����

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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CallAttack"));
		return;
	}
	// ���� ���� ����
	if (IsBoss)
	{
		// 20%Ȯ���� ��ų �ߵ�
		if (RandomValue <= 20)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Skill!!!!!!!!!!!!!!!!!"));
			PlayAnimMontage(SkillMontage, 1.3f);

			//// ��ų ���� ���
			//if (SkillSoundCue != nullptr)
			//{
			//	UGameplayStatics::SpawnSoundAttached(
			//		SkillSoundCue,
			//		GetRootComponent(),
			//		NAME_None,
			//		FVector::ZeroVector,
			//		EAttachLocation::KeepRelativeOffset,
			//		false,
			//		1.0f,  // Volume multiplier
			//		0.5f   // Pitch multiplier
			//	);
			//}
		}
		else
		{
			int aRandom = FMath::RandRange(0, 100);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack!@!@!@!@!@"));
			if (aRandom < 10 && aRandom >= 0)
			{
				if (AttackMontage.IsValidIndex(0))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("0"));
					PlayAnimMontage(AttackMontage[0], 1.0f);
				}
			}
			else if (aRandom < 20 && aRandom >= 10)
			{
				if (AttackMontage.IsValidIndex(1))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("1"));
					PlayAnimMontage(AttackMontage[1], 1.0f);
				}
			}
			else if (aRandom < 30 && aRandom >= 20)
			{
				if (AttackMontage.IsValidIndex(2))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("2"));
					PlayAnimMontage(AttackMontage[2], 1.0f);
				}
			}
			else if (aRandom < 40 && aRandom >= 30)
			{
				if (AttackMontage.IsValidIndex(3))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("3"));
					PlayAnimMontage(AttackMontage[3], 1.0f);
				}
			}
			else if (aRandom < 60 && aRandom >= 50)
			{
				if (AttackMontage.IsValidIndex(4))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("4"));
					PlayAnimMontage(AttackMontage[4], 1.0f);
				}
			}
			else if (aRandom < 70 && aRandom >= 60)
			{
				if (AttackMontage.IsValidIndex(5))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("5"));
					PlayAnimMontage(AttackMontage[5], 1.0f);
				}
			}
			else if (aRandom < 80 && aRandom >= 70)
			{
				if (AttackMontage.IsValidIndex(6))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("6"));
					PlayAnimMontage(AttackMontage[6], 1.0f);
				}
			}
			else if (aRandom < 90 && aRandom >= 80)
			{
				if (AttackMontage.IsValidIndex(7))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("7"));
					PlayAnimMontage(AttackMontage[7], 1.0f);
				}
			}
			else
			{
				if (AttackMontage.IsValidIndex(8))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("8"));
					PlayAnimMontage(AttackMontage[8], 1.0f);
				}
			}
		}
	}
	else // ��� �϶�
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("asdkjfyhasdfijhgi"));
		// ���� 2�� 
		if (RandomValue <= 50)
		{
			if (AttackMontage.IsValidIndex(0))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("0"));
				PlayAnimMontage(AttackMontage[0], 1.0f);
			}
		}
		else
		{
			if (AttackMontage.IsValidIndex(1))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("1"));
				PlayAnimMontage(AttackMontage[1], 1.0f);
			}
		}
	}


	IsAttacking = true;

	//���� ���� �̺�Ʈ �ߺ� ��� ����: ������ ���ε��� �̺�Ʈ�� ������ ����
	EDRAnim->OnAttackHitCheck.RemoveAll(this);




	//���� ���� �̺�Ʈ ���ε�
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	EDRAnim->OnMontageEnded.RemoveAll(this);  // �ߺ� ���ε� ����
	//���� ���� �� �̺�Ʈ ���ε�
	EDRAnim->OnAttackEnd.AddUObject(this, &AMyCharacter::AttackCheckEnd);



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
	//TargetSpeed = 0.0f;
	//CurrentSpeed = 0.0f;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack Animation Ended"));

	OnAttackEnd.Broadcast();
}





//  ���� ����


void AMyCharacter::AttackCheck()
{
	CurrentWeapon->StartAttack();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("RightHand"));
	int RandomSound = FMath::RandRange(0, 2);
	// ���� ���� ���		
	//if (RandomSound < 33)
	//{
	if (AttackSoundCue[RandomSound] != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(
			AttackSoundCue[RandomSound],
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			false,
			1.0f,  // Volume multiplier
			1.0f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
		);
	}
	//}
	//else if (RandomSound < 66)
	//{
	//	if (AttackSoundCue[1] != nullptr)
	//	{
	//		UGameplayStatics::SpawnSoundAttached(
	//			AttackSoundCue[1],
	//			GetRootComponent(),
	//			NAME_None,
	//			FVector::ZeroVector,
	//			EAttachLocation::KeepRelativeOffset,
	//			false,
	//			2.5f,  // Volume multiplier
	//			0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
	//		);
	//	}
	//}
	//else
	//{
	//	if (AttackSoundCue[2] != nullptr)
	//	{
	//		UGameplayStatics::SpawnSoundAttached(
	//			AttackSoundCue[2],
	//			GetRootComponent(),
	//			NAME_None,
	//			FVector::ZeroVector,
	//			EAttachLocation::KeepRelativeOffset,
	//			false,
	//			2.5f,  // Volume multiplier
	//			0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
	//		);
	//	}
	//}

//	// ��ų ����
//	if (RandomValue <= 30)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SkillCheck~~~~~~~~~~~~~~~~~~"));
//		FHitResult HitResult;
//		FCollisionQueryParams Params(NAME_None, false, this);
//
//		// ��ü�� �߽��� ĳ���� �� �տ� ����
//		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//		FVector ForwardLocation = StartLocation + GetActorForwardVector() * SkillRange;
//
//		bool bResult = GetWorld()->SweepSingleByChannel(
//			HitResult, // ������ �浹�� Ž���� ��� ���� ������ ���� ����ü
//			StartLocation, // Ž�� ���� ��ġ
//			ForwardLocation,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2, // ���� �浹 ������ ����� Ʈ���̽� ä��
//			FCollisionShape::MakeSphere(SkillRadius), // Ž�� �����  ����
//			Params);
//
//		// ����� ��� ������ ���� ����
//#if ENABLE_DRAW_DEBUG
//		FVector TraceVec = GetActorForwardVector() * SkillRange;
//		FVector Center = StartLocation + TraceVec * 0.5f;  // ��ü�� �߽��� ����
//		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 1.0f;
//
//		// ����� ���
//		DrawDebugSphere(GetWorld(),
//			ForwardLocation,
//			SkillRadius,
//			12, // ���׸�Ʈ ��, �� �������� �ε巯����
//			DrawColor,
//			false,
//			DebugLifeTime);
//
//#endif
//		// ���� ������
//		if (bResult)
//		{
//			if (HitResult.GetActor() != nullptr)
//			{
//				// ��Ʈ ���� ���
//				if (HitSoundCue[1] != nullptr)
//				{
//					UGameplayStatics::SpawnSoundAttached(
//						HitSoundCue[1],
//						GetRootComponent(),
//						NAME_None,
//						FVector::ZeroVector,
//						EAttachLocation::KeepRelativeOffset,
//						false,
//						2.5f,  // Volume multiplier
//						0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
//					);
//				}
//				// ������ ���� ����
//				FDamageEvent DamageEvent;
//				HitResult.GetActor()->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
//			}
//		}
//	}
//	// ��������
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AttackCheck~~~~~~~~~~~~~~~~~~"));
//		FHitResult HitResult;
//		FCollisionQueryParams Params(NAME_None, false, this);
//
//		// ��ü�� �߽��� ĳ���� �� �տ� ����
//		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//		FVector ForwardLocation = StartLocation + GetActorForwardVector() * AttackRange;
//
//		bool bResult = GetWorld()->SweepSingleByChannel(
//			HitResult, // ������ �浹�� Ž���� ��� ���� ������ ���� ����ü
//			StartLocation, // Ž�� ���� ��ġ
//			ForwardLocation,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2, // ���� �浹 ������ ����� Ʈ���̽� ä��
//			FCollisionShape::MakeSphere(AttackRadius), // Ž�� �����  ����
//			Params);
//
//		// ����� ��� ������ ���� ����
//#if ENABLE_DRAW_DEBUG
//		FVector TraceVec = GetActorForwardVector() * AttackRange;
//		FVector Center = StartLocation + TraceVec * 0.5f;  // ��ü�� �߽��� ����
//		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 1.0f;
//
//		// ����� ���
//		DrawDebugSphere(GetWorld(),
//			ForwardLocation,
//			AttackRadius,
//			12, // ���׸�Ʈ ��, �� �������� �ε巯����
//			DrawColor,
//			false,
//			DebugLifeTime);
//
//#endif
//		// ���� ������
//		if (bResult)
//		{
//			if (HitResult.GetActor() != nullptr)
//			{
//				// ��Ʈ ���� ���
//				if (HitSoundCue[0] != nullptr)
//				{
//					UGameplayStatics::SpawnSoundAttached(
//						HitSoundCue[0],
//						GetRootComponent(),
//						NAME_None,
//						FVector::ZeroVector,
//						EAttachLocation::KeepRelativeOffset,
//						false,
//						2.5f,  // Volume multiplier
//						0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
//					);
//				}
//				// ������ ���� ����
//				FDamageEvent DamageEvent;
//				HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
//			}
//		}
//		else
//		{
//			if (MissSoundCue[0] != nullptr)
//			{
//				UGameplayStatics::SpawnSoundAttached(
//					MissSoundCue[0],
//					GetRootComponent(),
//					NAME_None,
//					FVector::ZeroVector,
//					EAttachLocation::KeepRelativeOffset,
//					false,
//					2.5f,  // Volume multiplier
//					0.7f   // Pitch multiplier, 0.5�� �����ϸ� ��� �ӵ��� �������� ������
//				);
//			}
//		}
//	}


}
void AMyCharacter::AttackCheckEnd()
{
		CurrentWeapon->StopAttack();
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