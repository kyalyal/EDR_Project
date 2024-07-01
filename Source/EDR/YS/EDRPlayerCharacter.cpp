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

float AEDRPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
    
    UpdateHP(DamageAmount);


    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AEDRPlayerCharacter::SetHP(float NewHP)
{
    HP = NewHP;
}

void AEDRPlayerCharacter::UpdateHP(float NewHP)
{
    HP += NewHP;
}
