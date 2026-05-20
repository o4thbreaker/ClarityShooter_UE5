// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/CSmartObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Clarity.h"
#include "AI/CAIManager.h"

ACAIController::ACAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	/* Configure default Sight Config */

	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 80.f;

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	/* Register senses */
	PerceptionComponent->ConfigureSense(*Sight);
	PerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());

	/* ========= DEFAULT VALUES ========= */
	DetectionRate = 1.0f;
	DetectionThreshold = 5.0f;
	Target = nullptr;
	LastStimulusLocation = FVector::ZeroVector;
	AIManager = nullptr;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	/// \NOTE: done in OnPossess
	//AICharacter = Cast<ACAICharacter>(GetPawn());
	//if (!ensure(AICharacter)) return;

	/// \FIXME: maybe not the brightest idea
	AICharacter->AIController = this;

	if (AICharacter->SmartObject)
	{
		FGameplayTag SubTag;
		BTComponent->SetDynamicSubtree(SubTag, AICharacter->SmartObject->SubTree);
	} 
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerception);

	AICharacter = Cast<ACAICharacter>(InPawn);
	if (ensure(AICharacter))
	{
		AICharacter->Initialize();

		if (ensureMsgf(AICharacter->GetBehaviorTree(), TEXT("Behaviour Tree is nullptr. Please assgin BehaviourTree in your AICharacter")))
		{
			BlackboardComponent->InitializeBlackboard(*AICharacter->GetBehaviorTree()->BlackboardAsset);
			BTComponent->StartTree(*AICharacter->GetBehaviorTree());
		}
	}
}


void ACAIController::OnPerception(AActor* SpottedActor, FAIStimulus Stimulus)
{
	// if agent can see target - true, false otherwise
	if (UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus) == UAISense_Sight::StaticClass())
	{
		// if our character is hostile towards spotted actor
		if (AICharacter && AICharacter->IsHostile(SpottedActor))
		{
			GetBlackboardComponent()->SetValueAsBool(TEXT("Contact"), Stimulus.WasSuccessfullySensed());

			if (Stimulus.WasSuccessfullySensed())
			{
				SetTargetActor(SpottedActor);
				LastStimulusLocation = Stimulus.StimulusLocation;
				StimulusTimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
			}
		}

		// if agent is not detecting rn AND is looking at someone AND in idle state
		if (!GetWorldTimerManager().IsTimerActive(DetectionTimer)
			&& GetBlackboardComponent()->GetValueAsBool(TEXT("Contact"))
			&& GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState")) == (uint8)ECAIState::Idle)
		{
			DetectionLevel = 0.f;

			// start detecting after some time
			GetWorldTimerManager().SetTimer(DetectionTimer, this, &ACAIController::SetDetectionLevel, DetectionRate, true, 0.f);
		}

		return;
	}
	
	// if agent is already attacking doesn't really matter what else he has sensed
	if (GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState")) == (uint8)ECAIState::Attack) return;

	/// \TODO: add handling other senses here

	// if sensed some other way the hostile actor - start investigating
	if (AICharacter && AICharacter->IsHostile(SpottedActor))
	{
		GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), (uint8)ECAIState::Investigate);
		GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), Stimulus.StimulusLocation);
	}
}

void ACAIController::SetTargetActor(AActor* NewTarget)
{
	// if current state is not attacking
	if (GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState")) != (uint8)ECAIState::Attack)
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTarget);
		//SetFocus(NewTarget);

		Target = NewTarget;
	}
}

AActor* ACAIController::GetCurrentTarget() const
{
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
}


void ACAIController::SetDetectionLevel()
{
	// if no target or any contact whatsoever 
	if (!Target || !GetBlackboardComponent()->GetValueAsBool(TEXT("Contact")))
	{
		// if agent is doing anything but idling
		if (GetBlackboardComponent()->GetValueAsEnum(TEXT("AIState")) != (uint8)ECAIState::Idle)
		{
			GetWorldTimerManager().ClearTimer(DetectionTimer);
			return;
		}
		
		// there is no target nor contact so we're trying to decrement DetectionLevel if there is some
		if (DetectionLevel > 0.f)
		{
			DetectionLevel--;
			return;
		}

		// anyway clear the timer
		GetWorldTimerManager().ClearTimer(DetectionTimer);
		return;
	}

	const float Distance = GetPawn()->GetDistanceTo(Target);

	// if target is close enough - increase detection rate, otherwise keep it default
	DetectionRate = Distance <= 500.f ? 1.f : 2.f;
	DetectionLevel++;

	if (DetectionLevel >= DetectionThreshold)
	{
		// tell every other agent to attack
		AIManager->NotifyAIState(ECAIState::Attack);

		GetWorldTimerManager().ClearTimer(DetectionTimer);

		return;
	}
	
	// if the detection level is more than 50% - start investigating, otherwise keep detecting
	if (DetectionLevel >= DetectionThreshold / 2.f)
	{
		GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), (uint8)ECAIState::Investigate);
		GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), LastStimulusLocation);
	}
}

