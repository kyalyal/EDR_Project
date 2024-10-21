// Fill out your copyright notice in the Description page of Project Settings.
#include "EDRWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SceneComponent.h"


// Sets default values
AEDRWeaponBase::AEDRWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONMESH"));
    WeaponMesh->SetupAttachment(RootComponent);


    //기본 칼
    static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Weapon(TEXT("/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword"));
    if (SM_Weapon.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SM_Weapon.Object);
    }

    WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));


    //애니메이션
    WeaponAttackAnim = CreateDefaultSubobject<UAnimMontage>(TEXT("WEAPONATTACKANIM"));
    ConstructorHelpers::FObjectFinder<UAnimMontage>WA(TEXT("/Game/YS/Animation/ROG_Male/ROG_Sword/EDR_ROG_2H_Attack_A_Montage.EDR_ROG_2H_Attack_A_Montage"));
    if (WA.Succeeded())
    {
        WeaponAttackAnim = WA.Object;
    }

    //TracePoint
    AttackPointStart = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTSTART"));
    AttackPointEnd = CreateDefaultSubobject<USceneComponent>(TEXT("ATTACKPOINTEND"));

    AttackPointStart->SetupAttachment(WeaponMesh);
    AttackPointEnd->SetupAttachment(WeaponMesh);

    AttackPointStart->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
    AttackPointEnd->SetRelativeLocation(FVector(0.f, 0.f, 130.f));


    //데미지 초기화
    Damage = 20.f;


}

// Called when the game starts or when spawned
void AEDRWeaponBase::BeginPlay()
{
    Super::BeginPlay();



    if (!IsValid(WeaponAttackAnim))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s No Weapon Animation"), *GetName()));
    }

}

// Called every frame
void AEDRWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bAttacking)
    {
        TraceAttack();
    }

    
}

void AEDRWeaponBase::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}


void AEDRWeaponBase::OverlapActor(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  
    

}

void AEDRWeaponBase::AddIgnoreActor(AActor* IgnoreActor)
{
    IgnoreActors.Add(IgnoreActor);
}

void AEDRWeaponBase::StopAttack()
{
    bAttacking = false;

    IgnoreActors.Empty();
}

void AEDRWeaponBase::StartAttack()
{
    bAttacking = true;

}

void AEDRWeaponBase::TraceAttack()
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

