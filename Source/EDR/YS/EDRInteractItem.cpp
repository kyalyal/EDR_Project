// Fill out your copyright notice in the Description page of Project Settings.

#include "EDRInteractItem.h"
#include "Kismet/GameplayStatics.h"
#include "EDRInventory.h"
#include "EDRCharacter.h"


// Sets default values
AEDRInteractItem::AEDRInteractItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKELETALMESH"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetGenerateOverlapEvents(true);
	SkeletalMesh->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AEDRInteractItem::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 죽었을 때 델리게이트
	PlayerCharacter = Cast<AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnDeath.AddDynamic(this, &AEDRInteractItem::PlayerDie);
	}


	//데이터 에셋안에서 메시 입혀주기
	if (IsValid(InventoryStructure.DataAsset))
	{
		SkeletalMesh->SetSkeletalMesh(InventoryStructure.DataAsset->SK_Mesh);
	}
	else
	{
		//보류
		//GetWorld()->DestroyActor(this);
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

	UE_LOG(LogTemp, Log, TEXT("EDRInteractItem::PlayerInteract() - Interact Success."));

	//플레이어의 Inventory 시스템
	UEDRInventory* InventoryComp = PlayerCharacter->GetComponentByClass<UEDRInventory>();
	if (IsValid(InventoryComp))
	{
		InventoryComp->PickUpItem(this);

		GetWorld()->DestroyActor(this);
	}
}

void AEDRInteractItem::PlayerDie()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("EDRInteractItem::PlayerDie() - Success."));
}

