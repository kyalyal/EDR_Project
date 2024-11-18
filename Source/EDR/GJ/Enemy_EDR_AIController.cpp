// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_EDR_AIController.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BlackBoardKeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "EngineGlobals.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

const FName AEnemy_EDR_AIController::HomePosKey(TEXT("HomePos"));
const FName AEnemy_EDR_AIController::TargetLocation(TEXT("TargetLocation"));
const FName AEnemy_EDR_AIController::TargetKey(TEXT("Target"));
const FName AEnemy_EDR_AIController::Death = TEXT("Death");
const FName AEnemy_EDR_AIController::FightStartEnd = TEXT("FightStartEnd");

AEnemy_EDR_AIController::AEnemy_EDR_AIController(FObjectInitializer const& object_initializer)
{
	//ConstructorHelpers로 에디터에 미리 만들어둔 비헤이비어트리를 지정
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("BehaviorTree'/Game/GJ/AI/EneMy_Ai/Enemy_EDR_BehaviorTree.Enemy_EDR_BehaviorTree'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
	}
	else
	{
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	//Perception초기화
	SetPerceptionSystem();


}
void AEnemy_EDR_AIController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(btree))
    {
        // 트리가 유효하지 않을 경우 재시도
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Behavior Tree is not valid, retrying..."));
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemy_EDR_AIController::RetryLoadBehaviorTree, 1.0f, true);
    }
    else
    {
        // 유효하면 실행
        RunBehaviorTree(btree);
        behavior_tree_component->StartTree(*btree);
    }
}

void AEnemy_EDR_AIController::RetryLoadBehaviorTree()
{
    if (IsValid(btree))
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Behavior Tree loaded successfully!"));
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        
        RunBehaviorTree(btree);
        behavior_tree_component->StartTree(*btree);
    }
}
void AEnemy_EDR_AIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	if (!pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is null in OnPossess."));
		return;
	}

	if (!blackboard || !btree)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blackboard or Behavior Tree is null in OnPossess."));
		return;
	}

	// 비헤이비어 트리와 블랙보드 초기화
	blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	behavior_tree_component->StartTree(*btree);

	UE_LOG(LogTemp, Log, TEXT("AI Controller has possessed the pawn and initialized behavior tree and blackboard."));
}
UBlackboardComponent* AEnemy_EDR_AIController::get_blackboard() const
{
	return blackboard;
}

void AEnemy_EDR_AIController::OnUpdated(TArray<AActor*> const& updated_actors)
{
}
void AEnemy_EDR_AIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const player = Cast<AMyCharacter>(actor))
	{
		// 플레이어가 감지되면 블랙보드에 정보를 설정
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, Stimulus.WasSuccessfullySensed());

		// 플레이어 위치를 블랙보드에 저장
		if (Stimulus.WasSuccessfullySensed())
		{
			get_blackboard()->SetValueAsVector(bb_keys::target_location, player->GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Player detected, updating blackboard target location."));
		}
	}
}
// Ai Perception
void AEnemy_EDR_AIController::SetPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));


	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_EDR_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}