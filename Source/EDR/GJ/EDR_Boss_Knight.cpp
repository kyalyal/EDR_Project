// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Boss_Knight.h"
#include "EDR_Enemy_Weapon.h"

AEDR_Boss_Knight::AEDR_Boss_Knight()
{

	// 캐릭터 메시랑 캡슐콜리전
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);

	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MODEL(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Characters/Thral/SK_ThrallRigged.SK_ThrallRigged"));
	if (MODEL.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MODEL.Object);
	}
	SetActorScale3D(FVector(1.6f, 1.6f, 1.6f));
}
void AEDR_Boss_Knight::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocket(TEXT("hand_rSocket"));
	CurrentWeapon = GetWorld()->SpawnActor<AEDR_Enemy_Weapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		CurrentWeapon->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 180.0f)); // 180도 회전
		CurrentWeapon->SetActorRelativeLocation(FVector(-10.0f, 5.0f, 56.0f)); // 무기이동


		// 무기 크기 키우기
		CurrentWeapon->SetActorScale3D(FVector(2.0f));
	}
}
