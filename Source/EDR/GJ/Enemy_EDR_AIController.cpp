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

const FName AEnemy_EDR_AIController::HomePosKey(TEXT("HomePos"));
const FName AEnemy_EDR_AIController::TargetLocation(TEXT("TargetLocation"));

AEnemy_EDR_AIController::AEnemy_EDR_AIController(FObjectInitializer const& object_initializer)
{
	//ConstructorHelpers로 에디터에 미리 만들어둔 비헤이비어트리를 지정
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/All/Game/GJ/AI/EneMy_Ai/Enemy_BehaviorTree.Enemy_BehaviorTree'"));
	if (BTObject.Succeeded())
	{
		btree = BTObject.Object;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BT completed!"));
	}
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	//Perception초기화
	SetPerceptionSystem();


}

void AEnemy_EDR_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
}

void AEnemy_EDR_AIController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		//비헤이비어트리에 있는 블랙보드로 초기화
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}

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
		//성공적으로 감지하면 블랙보드에 true값을 넣어준다.
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, Stimulus.WasSuccessfullySensed());
	}
}

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