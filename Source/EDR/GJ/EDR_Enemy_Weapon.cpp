// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_Enemy_Weapon.h"

// Sets default values
AEDR_Enemy_Weapon::AEDR_Enemy_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;


	// 무기 메시
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;
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