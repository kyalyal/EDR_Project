// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Weapone.h"

// Sets default values
AEDR_Enemy_Weapone::AEDR_Enemy_Weapone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	// 메시 입히기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/BLACKSUN_BOSSBUNDLE1/Static_Meshes/Weapons/SM_Vanter_Sword.SM_Vanter_Sword"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void AEDR_Enemy_Weapone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEDR_Enemy_Weapone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

