// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_OverrideCombatState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAIController.h"

UCBTTask_OverrideCombatState::UCBTTask_OverrideCombatState()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_OverrideCombatState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (!MyController) return EBTNodeResult::Failed;
	if (!MyController->AIManager) return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum("CombatState", (uint8)DesiredState);
	return EBTNodeResult::Succeeded;
}

