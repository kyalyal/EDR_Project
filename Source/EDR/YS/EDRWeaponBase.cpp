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

    LineTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("LINETRACESTART"));
    LineTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("LINETRACEEND"));
    LineTraceStart->SetupAttachment(RootComponent);
    LineTraceEnd->SetupAttachment(RootComponent);

    LineTraceEnd->AddLocalOffset(FVector(0.f, 0.f, 130.f));

    TEnumAsByte<EObjectTypeQuery> TargetPawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
    ObjectTypes.Add(TargetPawn);
    
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

void AEDRWeaponBase::ApplyDamage()
{
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    IgnoreActors.Add(this);



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

void AEDRWeaponBase::StartAttack()
{

    UKismetSystemLibrary::LineTraceMultiForObjects(
        GetWorld(),
        GetActorLocation() + LineTraceStart->GetComponentLocation(),
        GetActorLocation() + LineTraceEnd->GetComponentLocation(),
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

