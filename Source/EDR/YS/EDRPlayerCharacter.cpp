// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


AEDRPlayerCharacter::AEDRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AEDRPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AEDRPlayerCharacter::Tick(float DeltaSecond)
{
    Super::Tick(DeltaSecond);
}


