// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Enemy_EDR_JAIController.generated.h"

/**
 * 
 */
UCLASS()
class EDR_API AEnemy_EDR_JAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_EDR_JAIController(FObjectInitializer const& object_initializer);
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


	// Ÿ�̸� �ڵ鷯
	FTimerHandle TimerHandle;

	// �񵿱� �ε��� ���� �ݹ� �Լ�
	void OnBehaviorTreeLoaded();

	// �����̺�� Ʈ�� �ε� ��õ� �Լ�
	void RetryLoadBehaviorTree();
public:
	static const FName HomePosKey;
	static const FName TargetLocation;
	static const FName TargetKey;
	static const FName Death;
	static const FName FightStartEnd;
	static const FName AbleAttack;

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& updated_actors);
	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	UFUNCTION()
	void SetPerceptionSystem();

	//AI Perception ����
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