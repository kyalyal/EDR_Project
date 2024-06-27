// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerCharacter.h"

void AEDRPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AEDRPlayerCharacter::Tick(float DeltaSecond)
{
    Super::Tick(DeltaSecond);
}

void AEDRPlayerCharacter::SetHP(float NewHP)
{
    HP = NewHP;
}
