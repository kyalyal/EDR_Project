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
	// 캡슐컴포넌트가 MyCharacter프리셋을 사용하도록 함
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	TargetLocation = GetActorLocation();

}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 캐릭터 회전 부드럽게
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
	
	//// 속도 보간
	CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, Acceleration); // Acceleration는 적절한 값으로 설정
	if (CurrentSpeed <= 10.0f)
	{
		CurrentSpeed = 0.0f;
	}
	// 캐릭터 이동 처리
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



// 캐릭터 정지 관련 함수
void AMyCharacter::StopMovement()
{
	// 이동을 멈추기 위해 MaxWalkSpeed를 0으로 설정
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

	// 2초 후에 이동을 재개
	GetWorld()->GetTimerManager().SetTimer(StopMovementTimerHandle, this, &AMyCharacter::ResumeMovement, 2.0f, false);
}

void AMyCharacter::ResumeMovement()
{
	// 원래의 이동 속도로 복원 (예: 200으로 설정)
	GetCharacterMovement()->MaxWalkSpeed = 200.0f; // 원하는 속도로 설정
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
	AttackCheckEnd();
	// 애니메이션 몽타주 실행
	PlayAnimMontage(DeathMontage, 1.0f);
	Death = true;
	// 스킬 사운드 재생
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
			0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
		);
	}
}
void AMyCharacter::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}




// 전투 시작

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
		EDRAnim->OnMontageEnded.RemoveAll(this);  // 기존 이벤트 제거
		EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnFightStartMontageEnded);
	}
}

// 전투 시작 애니메이션 종료 되면 호출
void AMyCharacter::OnFightStartMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// FightStart 애니메이션이 끝났을 때 공격 시작
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("bInterrupted: %s"), bInterrupted ? TEXT("True") : TEXT("False")));
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Fight Start Animation Ended"));
	TargetSpeed = 0.0f;
	CurrentSpeed = 0.0f;
	// 애니메이션 종료 확인
	IsFightStarting = true;
	//StopMovement();
	OnFightStartEnd.Broadcast();
}




// 공격 관련

void AMyCharacter::Attack()
{

	// 공격, 스킬 확률
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
	// 보스 공격 로직
	if (IsBoss)
	{
		// 20%확률로 스킬 발동
		if (RandomValue <= 20)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Skill!!!!!!!!!!!!!!!!!"));
			PlayAnimMontage(SkillMontage, 1.3f);

			//// 스킬 사운드 재생
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
	else // 잡몹 일때
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("asdkjfyhasdfijhgi"));
		// 공격 2개 
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

	//공격 판정 이벤트 중복 등록 방지: 기존에 바인딩된 이벤트가 있으면 제거
	EDRAnim->OnAttackHitCheck.RemoveAll(this);




	//공격 판정 이벤트 바인딩
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

	EDRAnim->OnMontageEnded.RemoveAll(this);  // 중복 바인딩 방지
	//공격 판정 끝 이벤트 바인딩
	EDRAnim->OnAttackEnd.AddUObject(this, &AMyCharacter::AttackCheckEnd);



	// 애니메이션 종료 시 공격 끝 처리
	EDRAnim->OnMontageEnded.RemoveAll(this);  // 중복 바인딩 방지
	EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);


}



// 공격 애니메이션 종료 처리 함수
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsAttacking)
	{
		return;
	}

	IsAttacking = false;

	// 공격 종료 시 이벤트 해제
	if (EDRAnim != nullptr)
	{
		EDRAnim->OnAttackHitCheck.RemoveAll(this);  // 중복 호출 방지
	}
	//TargetSpeed = 0.0f;
	//CurrentSpeed = 0.0f;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack Animation Ended"));

	OnAttackEnd.Broadcast();
}





//  공격 판정


void AMyCharacter::AttackCheck()
{
	CurrentWeapon->StartAttack();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("RightHand"));
	int RandomSound = FMath::RandRange(0, 2);
	// 공격 사운드 재생		
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
			1.0f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
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
	//			0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
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
	//			0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
	//		);
	//	}
	//}

//	// 스킬 판정
//	if (RandomValue <= 30)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SkillCheck~~~~~~~~~~~~~~~~~~"));
//		FHitResult HitResult;
//		FCollisionQueryParams Params(NAME_None, false, this);
//
//		// 구체의 중심을 캐릭터 발 앞에 설정
//		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//		FVector ForwardLocation = StartLocation + GetActorForwardVector() * SkillRange;
//
//		bool bResult = GetWorld()->SweepSingleByChannel(
//			HitResult, // 물리적 충돌이 탐지된 경우 관련 정보를 담을 구조체
//			StartLocation, // 탐색 시작 위치
//			ForwardLocation,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 트레이스 채널
//			FCollisionShape::MakeSphere(SkillRadius), // 탐색 사용할  도형
//			Params);
//
//		// 디버그 출력 정보를 담은 변수
//#if ENABLE_DRAW_DEBUG
//		FVector TraceVec = GetActorForwardVector() * SkillRange;
//		FVector Center = StartLocation + TraceVec * 0.5f;  // 구체의 중심을 설정
//		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 1.0f;
//
//		// 디버그 출력
//		DrawDebugSphere(GetWorld(),
//			ForwardLocation,
//			SkillRadius,
//			12, // 세그먼트 수, 더 많을수록 부드러워짐
//			DrawColor,
//			false,
//			DebugLifeTime);
//
//#endif
//		// 액터 감지시
//		if (bResult)
//		{
//			if (HitResult.GetActor() != nullptr)
//			{
//				// 히트 사운드 재생
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
//						0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
//					);
//				}
//				// 데미지 정보 전달
//				FDamageEvent DamageEvent;
//				HitResult.GetActor()->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
//			}
//		}
//	}
//	// 공격판정
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AttackCheck~~~~~~~~~~~~~~~~~~"));
//		FHitResult HitResult;
//		FCollisionQueryParams Params(NAME_None, false, this);
//
//		// 구체의 중심을 캐릭터 발 앞에 설정
//		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//		FVector ForwardLocation = StartLocation + GetActorForwardVector() * AttackRange;
//
//		bool bResult = GetWorld()->SweepSingleByChannel(
//			HitResult, // 물리적 충돌이 탐지된 경우 관련 정보를 담을 구조체
//			StartLocation, // 탐색 시작 위치
//			ForwardLocation,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 트레이스 채널
//			FCollisionShape::MakeSphere(AttackRadius), // 탐색 사용할  도형
//			Params);
//
//		// 디버그 출력 정보를 담은 변수
//#if ENABLE_DRAW_DEBUG
//		FVector TraceVec = GetActorForwardVector() * AttackRange;
//		FVector Center = StartLocation + TraceVec * 0.5f;  // 구체의 중심을 설정
//		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//		float DebugLifeTime = 1.0f;
//
//		// 디버그 출력
//		DrawDebugSphere(GetWorld(),
//			ForwardLocation,
//			AttackRadius,
//			12, // 세그먼트 수, 더 많을수록 부드러워짐
//			DrawColor,
//			false,
//			DebugLifeTime);
//
//#endif
//		// 액터 감지시
//		if (bResult)
//		{
//			if (HitResult.GetActor() != nullptr)
//			{
//				// 히트 사운드 재생
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
//						0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
//					);
//				}
//				// 데미지 정보 전달
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
//					0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
//				);
//			}
//		}
//	}


}
void AMyCharacter::AttackCheckEnd()
{
		CurrentWeapon->StopAttack();
}



// 데미지 받는 함수
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Death)
	{
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}


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