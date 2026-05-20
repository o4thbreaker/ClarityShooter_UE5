// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CBTTask_SelectCombatTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "CShooterInterface.h"

UCBTTask_SelectCombatTarget::UCBTTask_SelectCombatTarget()
{
	/// \TODO: switch to NodeMemory
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTTask_SelectCombatTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());

	if (!ensure(AIController)) return EBTNodeResult::Failed;

	if (EnemySeekerQuery)
	{
		// request is wrapper that is needed to execute eqs query
		EnemySeekerQueryRequest = FEnvQueryRequest(EnemySeekerQuery, AIController->GetAICharacter());
		
		// execute query with all matching, and when we get the result of eqs - run EnemySeekerQueryFinished()
		EnemySeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UCBTTask_SelectCombatTarget::EnemySeekerQueryFinished);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UCBTTask_SelectCombatTarget::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	BestTarget = nullptr;
	AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);

	float CurrentBestScore = 0.f;

	// index is needed to track actors in order of their indices in result items of eqs
	int32 Index = 0;
	TArray<AActor*> DetectedActors;

	Result->GetAllAsActors(DetectedActors);
	
	for (auto& Actor : DetectedActors)
	{
		if (ICShooterInterface* ShooterInterface = Cast<ICShooterInterface>(Actor))
		{
			if (ShooterInterface->GetFaction() != AIController->GetAICharacter()->GetFaction())
			{
				// if score of current actor (in eqs result array) is higher than biggest score
				if (Result->GetItemScore(Index) > CurrentBestScore && Result->GetItemScore(Index) > 0.f)
				{
					BestTarget = Actor;
					CurrentBestScore = Result->GetItemScore(Index);
				}
			}
		}
		// doesn't matter if actor implements shooter interface, if it is in array of detected actors - we increment
		Index++;
	}

	if (BestTarget)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", BestTarget);
	}
}
