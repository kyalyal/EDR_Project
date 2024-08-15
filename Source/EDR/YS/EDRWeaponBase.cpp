// Fill out your copyright notice in the Description page of Project Settings.
#include "EDRWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"


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

    LineTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("LINETRACESTART"));
    LineTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("LINETRACEEND"));
    LineTraceStart->SetupAttachment(WeaponMesh);
    LineTraceEnd->SetupAttachment(WeaponMesh);

    LineTraceEnd->AddLocalOffset(FVector(0.f, 0.f, 130.f));

    TEnumAsByte<EObjectTypeQuery> TargetPawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
    ObjectTypes.Add(TargetPawn);
    

    TraceIgnores.Add(this);

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

    StartAttack();
}

void AEDRWeaponBase::SetDamage(float NewDamage)
{
    Damage = NewDamage;
}

void AEDRWeaponBase::AddTraceIgnores(AActor* IgnoreActor)
{
    TraceIgnores.Add(IgnoreActor);
}

void AEDRWeaponBase::ApplyDamage()
{


    if (TraceHitResult.Num())
    {
        for (int32 i = 0; i < TraceHitResult.Num(); i++)
        {
            TraceHitResult[i].GetActor()->TakeDamage(Damage, DamageEvent(), GetInstigatorController(), this);
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

void AEDRWeaponBase::StartAttack()
{

    UKismetSystemLibrary::LineTraceMultiForObjects(
        GetWorld(),
        LineTraceStart->GetComponentLocation(),
        LineTraceEnd->GetComponentLocation(),
        ObjectTypes,
        false,
        TraceIgnores,
        EDrawDebugTrace::ForDuration,
        TraceHitResult,
        true
        );
    
}

void AEDRWeaponBase::StopAttack()
{
    
}

