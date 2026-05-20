// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CShooterInterface.h"

ACAIManager::ACAIManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACAIManager::BeginPlay()
{
	Super::BeginPlay();

	CreateAgentsList();
}

void ACAIManager::CreateAgentsList()
{
	TSubclassOf<ACAIController> ClassToFind = ACAIController::StaticClass();
	TArray<AActor*> Actors;

	// fill the array with all actors of class ACAIController
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, Actors);

	for (auto& Actor : Actors)
	{
		ACAIController* Controller = Cast<ACAIController>(Actor);
		if (Controller)
		{
			if (ICShooterInterface* ShooterInterface = Cast<ICShooterInterface>(Controller->GetAICharacter()))
			{
				if (ShooterInterface->GetFaction() == ManagerFaction)
				{
					Agents.AddUnique(Controller);
					Controller->AIManager = this;
				}
			}
		}
		
	}
}

void ACAIManager::NotifyAIState(ECAIState NewState)
{
	// change the AI state of all the agents
	for (auto& Controller : Agents)
	{
		Controller->GetBlackboardComponent()->SetValueAsEnum("AIState", (uint8)(NewState));
	}

	// if the state is attack, start the combat loop
	if (NewState == ECAIState::Attack)
	{
		GetWorldTimerManager().ClearTimer(SearchTimer);
		GetWorldTimerManager().SetTimer(CombatTimer, this, &ACAIManager::RunCombatLoop, 1.f, true);
		return;
	}

	// if the state is attack, start the search loop
	if (NewState == ECAIState::Search)
	{
		SearchTimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		GetWorldTimerManager().SetTimer(SearchTimer, this, &ACAIManager::RunSearchTimer, 1.f, true);
		return;
	}
}

void ACAIManager::RemoveAgent(ACAIController* AgentToRemove)
{
	int32 IndexToRemove = Agents.Find(AgentToRemove);
	if (IndexToRemove < 0) return;

	Agents.RemoveSingle(AgentToRemove);
}

void ACAIManager::RunSearchTimer()
{
	// if time left to search - return
	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - SearchTimeStamp < MaxStimulusTime_Search) return;

	// if time is up - clear the timer and tell all the agents to go to idle
	GetWorldTimerManager().ClearTimer(SearchTimer);
	NotifyAIState(ECAIState::Idle);
}

void ACAIManager::RunCombatLoop()
{
	if (Engaged())
	{
		/// \TODO: run approaching cover task
		return;
	}

	// if all of them are not engaged, then they lost the enemy
	NotifyAIState(ECAIState::LostEnemy);
	GetWorldTimerManager().ClearTimer(CombatTimer);
}

bool ACAIManager::Engaged()
{
	bool bIsEngaged = false;

	for (auto& Controller : Agents)
	{
		// if any of the agents has line of sight to contact
		if (Controller->GetBlackboardComponent()->GetValueAsBool("Contact"))
		{
			bIsEngaged = true;
			break;
		}

		// if any of the agents has line of sight to target
		if (Controller->GetBlackboardComponent()->GetValueAsObject("Target"))
		{
			bIsEngaged = true;
			break;
		}

		// if the time from the last stimulus is less than max stimulus time
		if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - Controller->GetStimulusTimeStamp() < MaxStimulusTime_Combat)
		{
			bIsEngaged = true;
			break;
		}
	}

	// if true then all agents are in combat loop, otherwise they are in search loop
	return bIsEngaged;
}

