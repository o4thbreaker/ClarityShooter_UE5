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

	MaxStimulusTime_Combat = 10.f;
	MaxStimulusTime_Search = 10.f;
	SearchTimeStamp = 0.f;
	AgentProvidingCoverFire = nullptr;
	ApproacherIndex = 0;
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

					if (Controller->GetAICharacter()->CombatRole == ECCombatRole::Defender)
					{
						Defenders.AddUnique(Controller->GetAICharacter());
					}
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

	if (Defenders.Find(AgentToRemove->GetAICharacter()))
	{
		Defenders.RemoveSingle(AgentToRemove->GetAICharacter());
	}
}

void ACAIManager::RequestCoverFire(bool ShouldProvideCoverFire, ACAICharacter* RequestInstigator)
{
	// if instigator is not valid or is not defender - return
	if (!RequestInstigator || Defenders.Find(RequestInstigator) < 0) return;

	if (ShouldProvideCoverFire)
	{
		// tell all the defenders
		for (auto& Defender : Defenders) 
		{
			// if the defender is not providing cover fire and is holding cover - tell him to provide cover fire
			if (Defender != RequestInstigator)
			{
				AgentProvidingCoverFire = Defender;
				Defender->AIController->SetShouldShootFromCover(true);
				break;
			}
		}
		// cover man is found - return
		return;
	}

	// if no one is providing cover fire - stop shooting from the previous one
	AgentProvidingCoverFire->AIController->SetShouldShootFromCover(false);
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
		// Approach
		if (Defenders.Num() > 0)
		{
			Defenders[ApproacherIndex]->AIController->GetBlackboardComponent()->SetValueAsEnum("CombatState", (uint8)ECCombatState::ApproachingCover);
			// request cover fire while approaching cover
			RequestCoverFire(true, Defenders[ApproacherIndex]);
			// if index is not the last one - increment
			ApproacherIndex = (ApproacherIndex + 1 <= Defenders.Num() - 1) ? ApproacherIndex++ : 0;
		}

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
		if (Controller->GetTargetActor())
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

