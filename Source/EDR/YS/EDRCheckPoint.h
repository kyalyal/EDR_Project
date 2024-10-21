// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EDRInteractItem.h"
#include "EDRCheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEDRCheckPoint : public AActor, public IEDRPlayerInterface
{
	GENERATED_BODY()
	
public:

	AEDRCheckPoint();

	virtual void PlayerInteract() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "CheckPoint")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	

private:

	// 페이드 인을 실행하기 전 딜레이 후 호출될 함수
	void StartFadeInWithDelay();

	void RestartLevel();

	// 타이머 핸들러
	FTimerHandle FadeTimerHandle;



	



	TObjectPtr<class UEDRGameInstance> EDRGameInstance;
};
