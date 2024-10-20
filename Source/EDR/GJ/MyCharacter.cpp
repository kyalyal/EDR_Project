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
	// 공격 범위 변수 초기화
	AttackRange = 400.0f;
	AttackRadius = 200.0f;
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 캐릭터 회전 부드럽게
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

// 공격 애니메이션 종료
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
void AMyCharacter::UpdateHP(float NewHP)
{
	hp += NewHP;

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UpdateHP() - %s HP : %f"), *GetName(), hp));
}

void AMyCharacter::Attack()
{

	// Death가 true 일경우 공격 애니메이션 정지 
	
	if (Death)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("is Death true"));
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
	PlayAnimMontage(AttackMontage, 0.5f);
	IsAttacking = true;
	// 공격 판정
	EDRAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
	// 공격 종료
	EDRAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
}

//  공격 판정
void AMyCharacter::AttackCheck()
{
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
			// 데미지 정보 전달
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
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
