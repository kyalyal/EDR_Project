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
	// 캡슐컴포넌트가 MyCharacter프리셋을 사용하도록 함
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));


}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 캐릭터 회전 부드럽게
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

void AMyCharacter::FightStart()
{
	PlayAnimMontage(FightStartMontage, 1.0f);
}

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
		return;
	}

	// 20%확률로 스킬 발동
	if (RandomValue <= 20)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Skill!!!!!!!!!!!!!!!!!"));
		PlayAnimMontage(SkillMontage, 1.3f);

		// 스킬 사운드 재생
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
				0.5f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
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

		// 공격 사운드 재생
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
				0.5f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
			);
		}
	}

	IsAttacking = true;

	// 공격 판정 이벤트 중복 등록 방지: 기존에 바인딩된 이벤트가 있으면 제거
	EDRAnim->OnAttackHitCheck.RemoveAll(this);

	// 공격 판정 이벤트 바인딩
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);

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

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Attack Animation Ended"));

	OnAttackEnd.Broadcast();
}
//  공격 판정
void AMyCharacter::AttackCheck()
{
	// 스킬 판정
	if (RandomValue <= 30)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("SkillCheck~~~~~~~~~~~~~~~~~~"));
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		// 구체의 중심을 캐릭터 발 앞에 설정
		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector ForwardLocation = StartLocation + GetActorForwardVector() * SkillRange;

		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult, // 물리적 충돌이 탐지된 경우 관련 정보를 담을 구조체
			StartLocation, // 탐색 시작 위치
			ForwardLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 트레이스 채널
			FCollisionShape::MakeSphere(SkillRadius), // 탐색 사용할  도형
			Params);

		// 디버그 출력 정보를 담은 변수
#if ENABLE_DRAW_DEBUG
		FVector TraceVec = GetActorForwardVector() * SkillRange;
		FVector Center = StartLocation + TraceVec * 0.5f;  // 구체의 중심을 설정
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 1.0f;

		// 디버그 출력
		DrawDebugSphere(GetWorld(),
			ForwardLocation,
			SkillRadius,
			12, // 세그먼트 수, 더 많을수록 부드러워짐
			DrawColor,
			false,
			DebugLifeTime);

#endif
		// 액터 감지시
		if (bResult)
		{
			if (HitResult.GetActor() != nullptr)
			{
				// 히트 사운드 재생
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
						0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
					);
				}
				// 데미지 정보 전달
				FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(SkillDamage, DamageEvent, GetController(), this);
			}
		}
	}
	// 공격판정
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("AttackCheck~~~~~~~~~~~~~~~~~~"));
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		// 구체의 중심을 캐릭터 발 앞에 설정
		FVector StartLocation = GetActorLocation() + FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector ForwardLocation = StartLocation + GetActorForwardVector() * AttackRange;

		bool bResult = GetWorld()->SweepSingleByChannel(
			HitResult, // 물리적 충돌이 탐지된 경우 관련 정보를 담을 구조체
			StartLocation, // 탐색 시작 위치
			ForwardLocation,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2, // 물리 충돌 감지에 사용할 트레이스 채널
			FCollisionShape::MakeSphere(AttackRadius), // 탐색 사용할  도형
			Params);

		// 디버그 출력 정보를 담은 변수
#if ENABLE_DRAW_DEBUG
		FVector TraceVec = GetActorForwardVector() * AttackRange;
		FVector Center = StartLocation + TraceVec * 0.5f;  // 구체의 중심을 설정
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 1.0f;

		// 디버그 출력
		DrawDebugSphere(GetWorld(),
			ForwardLocation,
			AttackRadius,
			12, // 세그먼트 수, 더 많을수록 부드러워짐
			DrawColor,
			false,
			DebugLifeTime);

#endif
		// 액터 감지시
		if (bResult)
		{
			if (HitResult.GetActor() != nullptr)
			{
				// 히트 사운드 재생
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
						0.7f   // Pitch multiplier, 0.5로 설정하면 재생 속도가 절반으로 느려짐
					);
				}
				// 데미지 정보 전달
				FDamageEvent DamageEvent;
				HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
			}
		}
	}


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