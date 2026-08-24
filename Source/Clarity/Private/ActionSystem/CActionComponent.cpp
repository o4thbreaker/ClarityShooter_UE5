// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CActionComponent.h"
#include "ActionSystem/CAction.h"

UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

/// \TODO: switch to Interface call, temporary solution
UCActionComponent* UCActionComponent::GetActionComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCActionComponent>(FromActor->GetComponentByClass(UCActionComponent::StaticClass()));
	}

	return nullptr;
}

void UCActionComponent::CancelActionsWithTags(AActor* Instigator, const FGameplayTagContainer& CancelTags, UCAction* ActionToIgnore)
{
	if (CancelTags.IsEmpty()) return;

	TArray<UCAction*> ActionsToInspect = Actions;

	for (UCAction* Action : ActionsToInspect)
	{
		if (!Action || Action == ActionToIgnore || !Action->IsRunning()) continue;

		if (CancelTags.HasTag(Action->ActionTag))
		{
			UE_LOG(LogTemp, Log, TEXT("ActionComponent: %s canceled because of tag matching"), *GetNameSafe(Action));
			Action->StopAction(Instigator);
		}
	}
}

UCAction* UCActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass)
{
	// check if action class is valid
	if (!ensure(ActionClass)) return nullptr;

	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bIsAutoStart && ensure(NewAction->CanStartAction(Instigator)))
		{
			CancelActionsWithTags(Instigator, NewAction->GetCancelActionWithTags(), NewAction);

			NewAction->StartAction(Instigator);
		}

		return NewAction;
	}

	return nullptr;
}

void UCActionComponent::RemoveAction(AActor* Instigator, UCAction* ActionToRemove)
{
	if (!ensure(ActionToRemove)) return;

	if (ActionToRemove->IsRunning())
	{
		ActionToRemove->StopAction(Instigator);
	}

	Actions.Remove(ActionToRemove);
}

bool UCActionComponent::StartActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString InstigatorName = Instigator ? Instigator->GetName() : "None";
				UE_LOG(LogTemp, Warning, TEXT("Failed to start action %s for %s"), *Action->ActionName.ToString(), *InstigatorName);
				continue;
			}

			CancelActionsWithTags(Instigator, Action->GetCancelActionWithTags(), Action);

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCActionComponent::StopActionByTag(AActor* Instigator, FGameplayTag ActionTag)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionTag == ActionTag)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

bool UCActionComponent::IsInActions(TSubclassOf<UCAction> ActionClassToCheck) const
{
	for (UCAction* Action : Actions)
	{
		if (Action->GetClass() == ActionClassToCheck)
		{
			return true;
		}
	}

	return false;
}

void UCActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	for (UCAction* Action : Actions)
	{
		if (Action && Action->bWantsTick && (Action->IsRunning() || Action->IsStopping()))
		{
			Action->TickAction(DeltaTime);
		}
	}
}