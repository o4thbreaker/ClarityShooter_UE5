// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_GetPathPoints.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "AI/CAICharacter.h"
#include "AI/CSmartObject_PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTask_GetPathPoints::UCBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Index = 0;
	/// \TODO: change to Memory-based
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_GetPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACAICharacter* AICharacter = Cast<ACAICharacter>(AIController->GetPawn());
	if (!AICharacter) return EBTNodeResult::Failed;

	ACSmartObject_PatrolPath* PatrolPath = Cast<ACSmartObject_PatrolPath>(AICharacter->SmartObject);
	if (!PatrolPath || PatrolPath->Locations.Num() < 1)
	{
		UE_LOG(LogTemp, Log, TEXT("No PatrolPath (or locations are not populated) on GetPathPoints Task"));
		return EBTNodeResult::Succeeded;
	}

	/// \TODO: fix the string hardcode
	// set the next MoveTo location to the nex location in patrol path array
	OwnerComp.GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PatrolPath->Locations[Index]);

	// if we are not at the end of the array, increment index, otherwise reset to 0
	if (Index < PatrolPath->Locations.Num() - 1)
	{
		Index++;
		return EBTNodeResult::Succeeded;
	}

	Index = 0;
	return EBTNodeResult::Succeeded;
}
