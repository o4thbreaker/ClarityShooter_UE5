// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIManagerSubsystem.h"
#include "AI/CAIController.h"
#include "AI/CAICharacter.h"
#include "CShooterInterface.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

UCAIManagerSubsystem::UCAIManagerSubsystem()
{
	MaxStimulusTime_Combat = 10.f;
	MaxStimulusTime_Search = 10.f;
	SearchTimeStamp = 0.f;
	AgentProvidingCoverFire = nullptr;
	ApproacherIndex = 0;
	ManagerFaction = ECFaction::Enemy;
}

void UCAIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("%s AIManager Subsystem has been initialized!"), *UEnum::GetValueAsString(ManagerFaction));
}

void UCAIManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	UE_LOG(LogTemp, Warning, TEXT("%s AIManager Subsystem OnWorldBeginPlay has been called!"), *UEnum::GetValueAsString(ManagerFaction));

	CreateAgentsList();
}

void UCAIManagerSubsystem::CreateAgentsList()
{
	TSubclassOf<ACAIController> ClassToFind = ACAIController::StaticClass();
	TArray<AActor*> Actors;

	// fill the array with all actors of class ACAIController
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, Actors);

	for (AActor*& Actor : Actors)
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

void UCAIManagerSubsystem::NotifyAIState(ECAIState NewState)
{
	// change the AI state of all the agents
	for (ACAIController*& Controller : Agents)
	{
		Controller->TrySetAIState(NewState);
	}

	// if the state is attack, start the combat loop
	if (NewState == ECAIState::Attack)
	{
		GetWorld()->GetTimerManager().ClearTimer(SearchTimer);
		GetWorld()->GetTimerManager().SetTimer(CombatTimer, this, &UCAIManagerSubsystem::RunCombatLoop, 1.f, true);
		return;
	}

	// if the state is attack, start the search loop
	if (NewState == ECAIState::Search)
	{
		SearchTimeStamp = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		GetWorld()->GetTimerManager().SetTimer(SearchTimer, this, &UCAIManagerSubsystem::RunSearchTimer, 1.f, true);
		return;
	}
}

void UCAIManagerSubsystem::RunCombatLoop()
{
	if (IsEngaged())
	{
		// approach if not in hold cover
		if (Defenders.Num() > 0 && Defenders[ApproacherIndex]->AIController->GetCombatState() != (uint8)ECCombatState::HoldCover)
		{
			Defenders[ApproacherIndex]->AIController->SetCombatState(ECCombatState::ApproachingCover);
			// request cover fire while approaching cover
			RequestCoverFire(true, Defenders[ApproacherIndex]);
			// if index is not the last one - increment
			ApproacherIndex = (ApproacherIndex + 1 <= Defenders.Num() - 1) ? ++ApproacherIndex : 0;
		}

		return;
	}

	// if all of them are not engaged, then they lost the enemy
	NotifyAIState(ECAIState::LostEnemy);
	GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
}

void UCAIManagerSubsystem::RunSearchTimer()
{
	// if time left to search - return
	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) - SearchTimeStamp < MaxStimulusTime_Search) return;

	// if time is up - clear the timer and tell all the agents to go to idle
	GetWorld()->GetTimerManager().ClearTimer(SearchTimer);
	NotifyAIState(ECAIState::Idle);
}

void UCAIManagerSubsystem::RequestCoverFire(bool ShouldProvideCoverFire, ACAICharacter* RequestInstigator)
{
	// if instigator is not valid or is not defender - return
	if (!RequestInstigator || Defenders.Find(RequestInstigator) < 0) return;

	if (ShouldProvideCoverFire)
	{
		// tell all the defenders
		for (ACAICharacter*& Defender : Defenders)
		{
			// if the defender is not providing cover fire and is holding cover - tell him to provide cover fire
			if (Defender && Defender != RequestInstigator && Defender->AIController->GetCombatState() == (uint8)ECCombatState::HoldCover)
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
	if (AgentProvidingCoverFire && AgentProvidingCoverFire->AIController->GetCombatState() == (uint8)ECCombatState::HoldCover)
	{
		AgentProvidingCoverFire->AIController->SetShouldShootFromCover(false);
	}
}

void UCAIManagerSubsystem::RemoveAgent(ACAIController* AgentToRemove)
{
	int32 IndexToRemove = Agents.Find(AgentToRemove);
	if (IndexToRemove < 0) return;

	Agents.RemoveSingle(AgentToRemove);

	if (Defenders.Find(AgentToRemove->GetAICharacter()))
	{
		Defenders.RemoveSingle(AgentToRemove->GetAICharacter());
	}
}

bool UCAIManagerSubsystem::IsEngaged()
{
	bool bIsEngaged = false;

	for (ACAIController*& Controller : Agents)
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

