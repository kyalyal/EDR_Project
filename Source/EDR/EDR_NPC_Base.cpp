// Fill out your copyright notice in the Description page of Project Settings.


#include "EDR_NPC_Base.h"

// Sets default values
AEDR_NPC_Base::AEDR_NPC_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEDR_NPC_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEDR_NPC_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEDR_NPC_Base::PlayerInteract()
{
	Interact();
}


void AEDR_NPC_Base::Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerInteract_Implementation called on AEDR_NPC_Base!"));
}

