// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Weapon.h"

// Sets default values
AEDR_Enemy_Weapon::AEDR_Enemy_Weapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// 무기 메시
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Vanter_Sword.SM_Vanter_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void AEDR_Enemy_Weapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEDR_Enemy_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

