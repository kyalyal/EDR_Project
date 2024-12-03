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


	// ���� �޽�
	//Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPONMESH"));
	WeaponMesh->SetupAttachment(RootComponent);

	//// �⺻ Į
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





	//������ �ʱ�ȭ
	Damage = 20.f;


}


void AEDR_Enemy_Weapon::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    // ������ ���� (GetOwner�� ���⸦ ������ ���͸� ��ȯ)
    WeaponOwner = GetOwner();
    if (WeaponOwner)
    {
        IgnoreActors.Add(WeaponOwner);
    }
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

   
    WeaponOwner = GetOwner();
    if (WeaponOwner && !IgnoreActors.Contains(WeaponOwner))
    {
        IgnoreActors.Add(WeaponOwner);
    }

}

void AEDR_Enemy_Weapon::TraceAttack()
{
    bool Trace;
    FHitResult OutHit;
    TArray<AActor*> ignores;

    // ���Ǿ� Ʈ���̽� ����
    FVector StartLocation = AttackPointStart->GetComponentLocation(); // Į�� ���� ���� (������)
    FVector EndLocation = AttackPointEnd->GetComponentLocation(); // Į�� �� ����
    FVector SphereCenter = (StartLocation + EndLocation) / 2.0f; // ���Ǿ� �߽�: Į���� �߰�
    float SphereRadius = FVector::Distance(StartLocation, EndLocation)/2.0f; // ���Ǿ� ������: Į�� ������ ����

    // ���Ǿ� Ʈ���̽� ����
    Trace = UKismetSystemLibrary::SphereTraceSingle(
        GetWorld(),
        SphereCenter,             // ���Ǿ� �߽�
        SphereCenter,             // ���۰� ���� �����ϰ� ����
        SphereRadius,             // ���Ǿ� ������
        UEngineTypes::ConvertToTraceType(ECC_PhysicsBody), // �浹 ä��
        false,                    // ������ Ʈ���̽� ��� ����
        IgnoreActors,                  // ������ ���� ���
        EDrawDebugTrace::ForDuration, // ����� �ð�ȭ ����
        OutHit,                   // �浹 ���
        true,                     // �ڽ� ���� ����
        FLinearColor::Red,        // Ʈ���̽� ����
        FLinearColor::Green,      // ��Ʈ ����
        3.f                       // ����� ǥ�� �ð�
    );
//bool Trace;
//FHitResult OutHit;
//TArray<AActor*> ignores;
//
//FVector StartLocation = AttackPointStart->GetComponentLocation();
//FVector EndLocation = AttackPointEnd->GetComponentLocation();
//
//    Trace = UKismetSystemLibrary::LineTraceSingle
//    (
//        GetWorld(),
//        StartLocation,
//        EndLocation,
//        UEngineTypes::ConvertToTraceType(ECC_PhysicsBody),
//        false,
//        ignores,
//        EDrawDebugTrace::ForDuration,
//        OutHit,
//        true,
//        FLinearColor::Red,
//        FLinearColor::Green,
//        3.f
//
//    );
//


    if (IgnoreActors.Contains(OutHit.GetActor())) return;
    if (OutHit.GetActor() == Player)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit!!!!!!!!!!"));
        if (OutHit.bBlockingHit)
        {
            // Ÿ���� ���� ���� (���⿡�� OtherActor ������)
            FVector HitDirection = OutHit.GetActor()->GetActorLocation() - GetActorLocation();


            UGameplayStatics::ApplyPointDamage(
                OutHit.GetActor(),            // ���ظ� ���� ����
                Damage,                // ������ ������ ��
                HitDirection,          // Ÿ���� ���� ����
                OutHit,           // �浹 ������ ���� FHitResult
                GetInstigatorController(),  // ���⸦ �ֵθ� ĳ������ ��Ʈ�ѷ�
                this,                  // �������� ���� ����
                UDamageType::StaticClass()  // �⺻ ������ Ÿ�� ���
            );

            IgnoreActors.Add(OutHit.GetActor());
        }
    }
}
