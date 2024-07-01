// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "EDRWeaponBase.h"
#include "Engine/DamageEvents.h"


// Sets default values
AEDRWeaponBase::AEDRWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONMESH"));


    AttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ATTACKCOLLECTION"));
    AttackCollision->SetupAttachment(RootComponent);
    AttackCollision->SetBoxExtent(FVector(100.f,100.f,100.f));
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

void AEDRWeaponBase::ApplyDamage()
{
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    IgnoreActors.Add(this);

    AttackCollision->GetOverlappingActors(OutActors);


    if (OutActors.Num())
    {
        for (int32 i = 0; i < OutActors.Num(); i++)
        {
            if (IgnoreActors.Contains(OutActors[i])) continue;

            OutActors[i]->TakeDamage(Damage, DamageEvent(), GetInstigatorController(), this);
        }
    }
}

FDamageEvent AEDRWeaponBase::DamageEvent()
{
    TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();

    FDamageEvent DamageEvent;
    DamageEvent.DamageTypeClass = DamageTypeClass;

    return DamageEvent;
}

