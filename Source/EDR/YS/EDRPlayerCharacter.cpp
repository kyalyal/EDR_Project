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
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    return 0;
}

void AEDRPlayerCharacter::ApplyDamage()
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    TArray<AActor*> IgnoreActors;
    TArray<AActor*> OutActors;

    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    IgnoreActors.Add(this);

    //추후 수정
    if (OutActors[0])
    {
        UGameplayStatics::ApplyDamage(OutActors[0], Damage, GetController(), nullptr, NULL);
    }
}

void AEDRPlayerCharacter::SetHP(float NewHP)
{
    HP = NewHP;
}
