// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTDecorator_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UCBTDecorator_CheckDistance::UCBTDecorator_CheckDistance()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, *NodeName, AActor::StaticClass());
	NodeName = "Check Distance";

	DistanceToCheck = 1000.f;
}

bool UCBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!ensure(MyController) || !ensure(MyBlackboard)) return false;

	AActor* Enemy = Cast<AActor>(MyBlackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (!Enemy) return false;

	// if distance from this ai to enemt is greater than DistanceToCheck - return true
	if (MyController->GetPawn()->GetDistanceTo(Enemy) >= DistanceToCheck) return true;

	return false;
}
