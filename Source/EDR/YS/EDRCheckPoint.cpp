// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRCheckPoint.h"
#include "EDRGameInstance.h"
#include "EDRCharacter.h"
#include "kismet/GameplayStatics.h"
#include "EDRGameViewportClient.h"

void AEDRCheckPoint::PlayerInteract()
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("EDRCheckPoint::PlayerInteract() - Interact Success."));

	

	EDRGameInstance = Cast<UEDRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	
	AEDRCharacter* PlayerRef = Cast<AEDRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	EDRGameInstance->SetPlayerStartLocation(PlayerRef->GetActorLocation());
	EDRGameInstance->SetPlayerStartRotation(PlayerRef->GetActorRotation());
	EDRGameInstance->SetControllerStartRotation(PlayerRef->GetController()->GetControlRotation());

	PlayerRef->PlayAnimMontage(PlayerRef->GetSitDownMontage());

	//페이드인 딜레이
	float DelayTime = 1.0f;  // 3초 딜레이
	GetWorld()->GetTimerManager().SetTimer(FadeTimerHandle, this, &AEDRCheckPoint::StartFadeInWithDelay, DelayTime, false);

	
}

void AEDRCheckPoint::StartFadeInWithDelay()
{
	// 페이드 아웃
	UEDRGameViewportClient* ViewportClient = Cast<UEDRGameViewportClient>(GetWorld()->GetGameViewport());

	if (ViewportClient)
	{

		ViewportClient->StartFadeOut(2.0f);

		ViewportClient->OnFadeInComplete = [this]()
			{
				RestartLevel();  // 페이드 인이 완료되면 레벨 재시작
			};


	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EDRCheckPoint::StartFadeInWithDelay - No Viewport"));
	}

}

void AEDRCheckPoint::RestartLevel()
{
	// 현재 레벨 이름을 가져옴
	FString CurrentLevelString = GetWorld()->GetMapName();
	CurrentLevelString = CurrentLevelString.Replace(TEXT("UEDPIE_0_"), TEXT(""));  // PIE 환경에서는 "UEDPIE_0_"이 붙을 수 있음


	FName CurrentLevelName(*CurrentLevelString);


	// 레벨을 다시 로드
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
}


