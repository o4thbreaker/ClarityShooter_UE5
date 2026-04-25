// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behaviour Tree is nullptr. Please assgin BehaviourTree in your AIController")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
