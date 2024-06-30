// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"


AEDRPlayerCharacter::AEDRPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("COLLECTIONSPHERE"));
    CollectionSphere->SetupAttachment(RootComponent);
    CollectionSphere->SetSphereRadius(100);

}

void AEDRPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AEDRPlayerCharacter::Tick(float DeltaSecond)
{
    Super::Tick(DeltaSecond);

    ApplyDamage();

}

float AEDRPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
    

    GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Green,TEXT("sssssssss"));

    return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


void AEDRPlayerCharacter::ApplyDamage()
{
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    IgnoreActors.Add(this);

    CollectionSphere->GetOverlappingActors(OutActors);


    if (OutActors.Num())
    {
        for (int32 i =0; i<OutActors.Num(); i++)
        {
            if (IgnoreActors.Contains(OutActors[i])) continue;

            OutActors[i]->TakeDamage(Damage,DamageEvent(), GetInstigatorController(), this);
        }
    }
}

FDamageEvent AEDRPlayerCharacter::DamageEvent()
{
    TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
    
    FDamageEvent DamageEvent;
    DamageEvent.DamageTypeClass = DamageTypeClass;
    
    return DamageEvent;
}

void AEDRPlayerCharacter::SetHP(float NewHP)
{
    HP = NewHP;
}
