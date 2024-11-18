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
	//ConstructorHelpers�� �����Ϳ� �̸� ������ �����̺��Ʈ���� ����
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

	//Perception�ʱ�ȭ
	SetPerceptionSystem();


}
void AEnemy_EDR_AIController::BeginPlay()
{
    Super::BeginPlay();

    if (!IsValid(btree))
    {
        // Ʈ���� ��ȿ���� ���� ��� ��õ�
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Behavior Tree is not valid, retrying..."));
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemy_EDR_AIController::RetryLoadBehaviorTree, 1.0f, true);
    }
    else
    {
        // ��ȿ�ϸ� ����
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

	// �����̺�� Ʈ���� ������ �ʱ�ȭ
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
		// �÷��̾ �����Ǹ� �����忡 ������ ����
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, Stimulus.WasSuccessfullySensed());

		// �÷��̾� ��ġ�� �����忡 ����
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