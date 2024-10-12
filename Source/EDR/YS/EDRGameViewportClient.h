// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "EDRGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API UEDRGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	// 매 프레임 호출되어 뷰포트를 그리는 함수 (여기서 페이드 효과를 적용)
	virtual void Draw(FViewport* InViewport, FCanvas* SceneCanvas) override;

	// 페이드 인 효과를 시작하는 함수
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeIn(float Duration);

	// 페이드 아웃 효과를 시작하는 함수
	UFUNCTION(BlueprintCallable, Category = "Fade")
	void StartFadeOut(float Duration);

private:
	// 현재 페이드 알파 값 (0.0 = 완전히 보이는 상태, 1.0 = 완전히 검은 상태)
	float FadeAlpha;

	// 페이드 속도 (지속 시간에 따른 속도 계산)
	float FadeSpeed;

	// 현재 페이드 인 또는 페이드 아웃 상태인지 여부를 저장하는 플래그
	bool bIsFadingIn;
	bool bIsFadingOut;

public:

	TFunction<void()> OnFadeInComplete;

};
