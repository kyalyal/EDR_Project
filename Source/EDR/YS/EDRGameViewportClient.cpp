// Fill out your copyright notice in the Description page of Project Settings.


#include "EDRGameViewportClient.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


void UEDRGameViewportClient::Draw(FViewport* InViewport, FCanvas* SceneCanvas)
{// 먼저 기본 뷰포트 내용을 그리기 위해 부모 클래스의 Draw 함수 호출
	Super::Draw(Viewport, SceneCanvas);

	// 페이드 인 또는 페이드 아웃 중 하나라도 활성화되어 있을 경우
	if (bIsFadingIn || bIsFadingOut)
	{
		// 델타 타임을 사용하여 알파 값을 조정 (프레임에 무관하게 일정한 속도로 변화)
		float DeltaTime = GetWorld()->GetDeltaSeconds();

		// 페이드 인 로직
		if (bIsFadingIn)
		{
			FadeAlpha -= FadeSpeed * DeltaTime;
			FadeAlpha = FMath::Clamp(FadeAlpha, 0.0f, 1.0f);  // 알파 값을 0과 1 사이로 제한

			// 알파 값이 0이 되면 페이드 인 종료
			if (FadeAlpha <= 0.0f)
			{
				bIsFadingIn = false;
			}
		}
		// 페이드 아웃 로직
		else if (bIsFadingOut)
		{
			FadeAlpha += FadeSpeed * DeltaTime;
			FadeAlpha = FMath::Clamp(FadeAlpha, 0.0f, 1.0f);  // 알파 값을 0과 1 사이로 제한

			// 알파 값이 1이 되면 페이드 아웃 종료
			if (FadeAlpha >= 1.0f)
			{
				bIsFadingOut = false;
				// 페이드 인 완료 후 호출할 함수가 있으면 실행
				if (OnFadeInComplete)
				{
					OnFadeInComplete();
				}
			}
		}

		// 현재 알파 값을 사용하여 화면에 검은색 오버레이를 그리기
		FCanvasTileItem TileItem(FVector2D(0, 0), FVector2D(Viewport->GetSizeXY()), FLinearColor(0, 0, 0, FadeAlpha));
		TileItem.BlendMode = SE_BLEND_Translucent;  // 투명도를 적용하기 위해 블렌드 모드 설정
		SceneCanvas->DrawItem(TileItem);
	}
}

void UEDRGameViewportClient::StartFadeIn(float Duration)
{
	if (Duration > 0.0f)
	{
		// 페이드 속도를 설정 (지속 시간에 따른 계산)
		FadeSpeed = 1.0f / Duration;
		FadeAlpha = 1.0f;  // 완전히 검은 상태에서 시작
		bIsFadingIn = true;  // 페이드 인 활성화
		bIsFadingOut = false;  // 페이드 아웃 비활성화
	}
}

void UEDRGameViewportClient::StartFadeOut(float Duration)
{
	if (Duration > 0.0f)
	{
		// 페이드 속도를 설정 (지속 시간에 따른 계산)
		FadeSpeed = 1.0f / Duration;
		FadeAlpha = 0.0f;  // 완전히 투명한 상태에서 시작
		bIsFadingIn = false;  // 페이드 인 비활성화
		bIsFadingOut = true;  // 페이드 아웃 활성화
	}
}
