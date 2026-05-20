// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_OverrideAIState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAIManager.h"
#include "AI/CAIController.h"

UCBTTask_OverrideAIState::UCBTTask_OverrideAIState()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_OverrideAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* MyController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (!ensure(MyController)) return EBTNodeResult::Failed;

	if (RunMode == ECRunMode::ThisAgent)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", (uint8)DesiredState);
	}
	else
	{
		MyController->AIManager->NotifyAIState(DesiredState);
	}

	return EBTNodeResult::Succeeded;
}
