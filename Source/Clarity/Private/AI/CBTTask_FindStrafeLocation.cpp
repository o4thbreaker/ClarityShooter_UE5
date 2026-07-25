// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_FindStrafeLocation.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "AI/CAIManagerSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTask_FindStrafeLocation::UCBTTask_FindStrafeLocation()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_FindStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (AIController && LocationSeekerQuery)
	{
		LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, AIController->GetAICharacter());
		LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UCBTTask_FindStrafeLocation::LocationSeekerQueryFinished);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UCBTTask_FindStrafeLocation::LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	int32 Index = 0;
	float CurrentBestScore = 0;
	TArray<FVector> Locations;
	Result->GetAllAsLocations(Locations);

	for (FVector& PotentialLocation : Locations)
	{
		if (IsDistanceGreater(PotentialLocation) && Result->GetItemScore(Index) > CurrentBestScore)
		{
			StrafeLocation = PotentialLocation;
			CurrentBestScore = Result->GetItemScore(Index);
		}
		Index++;
	}


	AIController->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", StrafeLocation);
}

bool UCBTTask_FindStrafeLocation::IsDistanceGreater(FVector Location)
{   
	if (AIController->AIManager->GetAgents().Num() <= 1) return true;

	bool ConsiderThisLocation = true;

	// calculate distance from given location and all of the agents of the team
	for (ACAIController* Agent : AIController->AIManager->GetAgents())
	{
		const float CalculatedDistance = (Location - Agent->GetAICharacter()->GetActorLocation()).Size();

		// if distance from this agent is higher than distance from certain agent - other guy is closer, so this location isn't suitable
		if (CalculatedDistance <= DistanceToTarget)
		{
			ConsiderThisLocation = false;
			break;
		}
	}

	return ConsiderThisLocation;
}

