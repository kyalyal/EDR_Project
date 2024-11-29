// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Weapon.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"

// Sets default values
AEDR_Enemy_Weapon::AEDR_Enemy_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;


	// 무기 메시
	//Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONMESH"));
	WeaponMesh->SetupAttachment(RootComponent);

	//// 기본 칼
 //   static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Vanter_Sword.SM_Vanter_Sword"));
 //   if (SK_WEAPON.Succeeded())
 //   {
 //       WeaponMesh->SetStaticMesh(SK_WEAPON.Object);
 //   }
 //   WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
    //TracePoint
    AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
    AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

    AttackPointStart->SetupAttachment(WeaponMesh);
    AttackPointEnd->SetupAttachment(WeaponMesh);

    //AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
    //AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 120.f));





	//데미지 초기화
	Damage = 20.f;


}

// Called when the game starts or when spawned
void AEDR_Enemy_Weapon::BeginPlay()
{
    Super::BeginPlay();

    Player = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

// Called every frame
void AEDR_Enemy_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (bAttacking)
    {
        
        TraceAttack();
    }
}


void AEDR_Enemy_Weapon::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}
void AEDR_Enemy_Weapon::OverlapActor(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{



}

void AEDR_Enemy_Weapon::AddIgnoreActor(AActor* IgnoreActor)
{
	IgnoreActors.Add(IgnoreActor);
}

void AEDR_Enemy_Weapon::StopAttack()
{
	bAttacking = false;
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("BBABBABBA"));
	IgnoreActors.Empty();
}


void AEDR_Enemy_Weapon::StartAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("AttackStart"));
	bAttacking = true;

}

void AEDR_Enemy_Weapon::TraceAttack()
{

    bool Trace;
    FHitResult OutHit;
    TArray<AActor*> ignores;

    FVector StartLocation = AttackPointStart->GetComponentLocation();
    FVector EndLocation = AttackPointEnd->GetComponentLocation();


    Trace = UKismetSystemLibrary::LineTraceSingle
    (
        GetWorld(),
        StartLocation,
        EndLocation,
        UEngineTypes::ConvertToTraceType(ECC_PhysicsBody),
        false,
        ignores,
        EDrawDebugTrace::ForDuration,
        OutHit,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        3.f

    );


    if (IgnoreActors.Contains(OutHit.GetActor())) return;
    if (OutHit.GetActor() == Player)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit!!!!!!!!!!"));
        if (OutHit.bBlockingHit)
        {
            // 타격이 들어온 방향 (무기에서 OtherActor 쪽으로)
            FVector HitDirection = OutHit.GetActor()->GetActorLocation() - GetActorLocation();


            UGameplayStatics::ApplyPointDamage(
                OutHit.GetActor(),            // 피해를 받을 액터
                Damage,                // 적용할 데미지 값
                HitDirection,          // 타격이 들어온 방향
                OutHit,           // 충돌 정보를 담은 FHitResult
                GetInstigatorController(),  // 무기를 휘두른 캐릭터의 컨트롤러
                this,                  // 데미지를 입힌 무기
                UDamageType::StaticClass()  // 기본 데미지 타입 사용
            );

            IgnoreActors.Add(OutHit.GetActor());
        }
    }
}
