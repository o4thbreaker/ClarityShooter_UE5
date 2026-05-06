// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "AI/CAICharacter.h"

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

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
