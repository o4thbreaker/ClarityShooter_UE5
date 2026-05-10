// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ACAIController::ACAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

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
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerception);

	ACAICharacter* AICharacter = Cast<ACAICharacter>(InPawn);
	if (ensure(AICharacter))
	{
		AICharacter->Initialize();
	}

	if (ensureMsgf(BehaviorTree, TEXT("Behaviour Tree is nullptr. Please assgin BehaviourTree in your AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}


void ACAIController::OnPerception(AActor* SpottedActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		SetTargetActor(SpottedActor);
	}
}

void ACAIController::SetTargetActor(AActor* NewTarget)
{
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewTarget);
	SetFocus(NewTarget);
}

AActor* ACAIController::GetCurrentTarget() const
{
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
}
