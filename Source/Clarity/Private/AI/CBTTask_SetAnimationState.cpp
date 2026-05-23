// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_SetAnimationState.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAICharacter.h"

UCBTTask_SetAnimationState::UCBTTask_SetAnimationState()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;

	NodeName = "Set Animation State";

	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
	bInCombat = false;
	bShouldFocus = false;
	bInCombat = false;
}

EBTNodeResult::Type UCBTTask_SetAnimationState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!ensure(MyController)) return EBTNodeResult::Failed;

	ACAICharacter* MyCharacter = Cast<ACAICharacter>(MyController->GetPawn());
	if (!ensure(MyCharacter)) return EBTNodeResult::Failed;

	MyCharacter->SetIsCombat(bInCombat);
	MyCharacter->SetIsCrouching(bIsCrouching);

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();

	AActor* Enemy = Cast<AActor>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (Enemy)
	{
		bShouldFocus ? MyController->SetFocus(Enemy) : MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		return EBTNodeResult::Succeeded;
	}

	MyController->ClearFocus(EAIFocusPriority::LastFocusPriority);
	return EBTNodeResult::Succeeded;
}
