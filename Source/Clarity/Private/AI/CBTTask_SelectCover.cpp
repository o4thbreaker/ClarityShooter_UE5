// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_SelectCover.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CCoverActor.h"

UCBTTask_SelectCover::UCBTTask_SelectCover()
{
	DesiredDirectness = 0.f;
	DesiredDistance = 500.f;

	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_SelectCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (AIController && ensure(CoverSeekerQuery))
	{
		CoverSeekerQueryRequest = FEnvQueryRequest(CoverSeekerQuery, AIController->GetAICharacter());
		CoverSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UCBTTask_SelectCover::CoverSeekerQueryFinished);
	}

	return EBTNodeResult::Succeeded;
}

void UCBTTask_SelectCover::CoverSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	SelectedCover = nullptr;
	AActor* Target = AIController->GetTargetActor();

	float CurrentBestScore = 0.f;
	int32 Index = 0;
	TArray<AActor*> DetectedActors;
	Result->GetAllAsActors(DetectedActors);

	if (RunMode == ECCoverType::Approach)
	{
		for (AActor*& Actor : DetectedActors)
		{
			ACCoverActor* Cover = Cast<ACCoverActor>(Actor);
			if (Cover )
			{
				if (Cover->GetDistanceTo(Target) >= DesiredDistance)
				{
					if (Cover->bIsCoverAvailable)
					{
						if (Cover->CurrentCharacter == nullptr)
						{
							const float ActorToTargetDistance = AIController->GetAICharacter()->GetDistanceTo(Target);
							const float CoverToTargetDistance = Cover->GetDistanceTo(Target);
							const float AgentToCoverDistance = AIController->GetAICharacter()->GetDistanceTo(Cover);

							/// \NOTE: check the Tactical Position Selection from Matthew Jack' book Game AI Pro
							const float CalculatedDirectness = (ActorToTargetDistance - CoverToTargetDistance) / AgentToCoverDistance;

							if (Result->GetItemScore(Index) > CurrentBestScore && CalculatedDirectness > DesiredDirectness)
							{
								SelectedCover = Cover;
								CurrentBestScore = Result->GetItemScore(Index);
							}
						}
					}
				}
			}
			Index++;
		}
	}

	// select any cover available and at least any specified distance from target
	if (RunMode == ECCoverType::Reposition)
	{
		if (Result->GetItemScore(Index) > CurrentBestScore)
		{
			for (AActor*& Actor : DetectedActors)
			{
				ACCoverActor* Cover = Cast<ACCoverActor>(Actor);
				if (Cover && Cover->bIsCoverAvailable && Cover->CurrentCharacter == nullptr && Result->GetItemScore(Index) > CurrentBestScore)
				{
					SelectedCover = Cover;
					CurrentBestScore = Result->GetItemScore(Index);
				}
				Index++;
			}
		}
	}

	if (RunMode == ECCoverType::Retreat)
	{
		for (AActor*& Actor : DetectedActors)
		{
			ACCoverActor* Cover = Cast<ACCoverActor>(Actor);
			if (Cover && Cover->GetDistanceTo(Target) <= DesiredDistance && Cover->bIsCoverAvailable && Cover->CurrentCharacter == nullptr)
			{
				const float ActorToTargetDistance = AIController->GetAICharacter()->GetDistanceTo(Target);
				const float CoverToTargetDistance = Cover->GetDistanceTo(Target);
				const float AgentToCoverDistance = AIController->GetAICharacter()->GetDistanceTo(Cover);

				/// \NOTE: check the Tactical Position Selection from Matthew Jack' book Game AI Pro
				const float CalculatedDirectness = (ActorToTargetDistance - CoverToTargetDistance) / AgentToCoverDistance;

				// we only consider those covers that are more distant from the target than the current position of the agent
				if (Result->GetItemScore(Index) > CurrentBestScore && CalculatedDirectness < DesiredDirectness)
				{
					SelectedCover = Cover;
					CurrentBestScore = Result->GetItemScore(Index);
				}
			}
			Index++;
		}
	}

	if (SelectedCover)
	{
		// reserve this cover for this agent

		ACCoverActor* PreviousCover = Cast<ACCoverActor>(AIController->GetBlackboardComponent()->GetValueAsObject("CoverActor"));
		if (PreviousCover && PreviousCover != SelectedCover)
		{
			PreviousCover->CurrentCharacter = nullptr;
			PreviousCover->bIsCoverAvailable = true;
		}

		AIController->GetBlackboardComponent()->SetValueAsObject("CoverActor", SelectedCover);
		SelectedCover->CurrentCharacter = AIController->GetAICharacter();
		SelectedCover->bIsCoverAvailable = false;
	}

}


