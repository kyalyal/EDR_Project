// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "EDR/YS/EDRCharacter.h"
#include "EDRInteractItem.h"

// Sets default values
AEDRInteractItem::AEDRInteractItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEDRInteractItem::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter = Cast<AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(PlayerCharacter))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("EDRInteractItem::BeginPlay - Player Cast Success."));
		PlayerCharacter->OnDeath.AddDynamic(this, &AEDRInteractItem::PlayerDie);

	}
}

// Called every frame
void AEDRInteractItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEDRInteractItem::PlayerInteract()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("EDRInteractItem::PlayerInteract() - Interact Success."));
}

void AEDRInteractItem::PlayerDie()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("EDRInteractItem::PlayerDie() - Success."));
}

