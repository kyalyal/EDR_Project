// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enemy_EDR_AIController.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEnemy_EDR_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_EDR_AIController(FObjectInitializer const& object_initializer);
	void BeginPlay() override;
	void OnPossess(APawn* pawn) override;

	class UBlackboardComponent* get_blackboard() const;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;
	class UAISenseConfig_Sight* SightConfig;


	// 타이머 핸들러
	FTimerHandle TimerHandle;

	// 비동기 로딩을 위한 콜백 함수
	void OnBehaviorTreeLoaded();

	// 비헤이비어 트리 로드 재시도 함수
	void RetryLoadBehaviorTree();
public:
	static const FName HomePosKey;
	static const FName TargetLocation;
	static const FName TargetKey;

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
	void SetPerceptionSystem();

	//AI Perception 변수
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation = 900.f;

};