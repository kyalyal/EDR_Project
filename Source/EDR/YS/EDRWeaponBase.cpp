// Fill out your copyright notice in the Description page of Project Settings.
#include "EDRWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "Components/BoxComponent.h"


// Sets default values
AEDRWeaponBase::AEDRWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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

}

// Called when the game starts or when spawned
void AEDRWeaponBase::BeginPlay()
{
    Super::BeginPlay();

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
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("%s - WeaponBase::OverlapActor - AttackTarget : %s"), *GetName(), *OtherActor->GetName()));

}

void AEDRWeaponBase::AddIgnoreActor(AActor* IgnoreActor)
{
    IgnoreActors.Add(IgnoreActor);
}

