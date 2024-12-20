// Fill out your copyright notice in the Description page of Project Settings.
#include "EDRWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"


// Sets default values
AEDRWeaponBase::AEDRWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONMESH"));
    WeaponMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_Weapon(TEXT("/Game/GKnight/Meshes/Weapon/SM_WP_GothicKnight_Sword.SM_WP_GothicKnight_Sword"));
    if (SM_Weapon.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SM_Weapon.Object);
    }

    WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));


    AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOMPONENT"));
    AttackCollision->SetupAttachment(WeaponMesh);
    AttackCollision->SetBoxExtent(FVector(5.f, 5.f, 60.f));
    AttackCollision->AddLocalOffset(FVector(0.f, 0.f, 80.f));
    AttackCollision->SetHiddenInGame(false);
    AttackCollision->SetCollisionProfileName(TEXT("TargetCharacter"));


    WeaponAttackAnim = CreateDefaultSubobject<UAnimMontage>(TEXT("WEAPONATTACKANIM"));
    ConstructorHelpers::FObjectFinder<UAnimMontage>WA(TEXT("/Game/YS/Animation/ROG_Male/ROG_Sword/EDR_ROG_2H_Attack_A_Montage.EDR_ROG_2H_Attack_A_Montage"));
    if (WA.Succeeded())
    {
        WeaponAttackAnim = WA.Object;
    }

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

}

void AEDRWeaponBase::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}


void AEDRWeaponBase::ApplyDamage(AActor* TargetActor)
{

     TargetActor->TakeDamage(Damage, DamageEvent(), GetInstigatorController(), this);
}

FDamageEvent AEDRWeaponBase::DamageEvent()
{
    TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

    FDamageEvent DamageEvent;
    DamageEvent.DamageTypeClass = DamageTypeClass;

    return DamageEvent;
}

void AEDRWeaponBase::StartAttack()
{
    AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEDRWeaponBase::OverlapActor);
   
    
}

void AEDRWeaponBase::StopAttack()
{
    AttackCollision->OnComponentBeginOverlap.RemoveDynamic(this, &AEDRWeaponBase::OverlapActor);
}

void AEDRWeaponBase::OverlapActor(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (IgnoreActors.Contains(OtherActor)) return;

    ApplyDamage(OtherActor);

}

void AEDRWeaponBase::AddIgnoreActor(AActor* IgnoreActor)
{
    IgnoreActors.Add(IgnoreActor);
}

