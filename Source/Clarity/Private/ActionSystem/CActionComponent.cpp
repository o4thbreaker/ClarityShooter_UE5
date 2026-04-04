// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CActionComponent.h"
#include "ActionSystem/CAction.h"

UCActionComponent::UCActionComponent()
{
	
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<UCAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UCActionComponent::AddAction(AActor* Instigator, TSubclassOf<UCAction> ActionClass)
{
	// check if action class is valid
	if (!ensure(ActionClass)) return;

	UCAction* NewAction = NewObject<UCAction>(GetOwner(), ActionClass);
	if (ensure(NewAction))
	{
		NewAction->Initialize(this);

		Actions.Add(NewAction);

		if (NewAction->bIsAutoStart && ensure(NewAction->CanStartAction(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UCActionComponent::RemoveAction(AActor* Instigator, UCAction* ActionToRemove)
{
	if (!ensure(ActionToRemove) && !ActionToRemove->IsRunning()) return;

	Actions.Remove(ActionToRemove);
}

bool UCActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStartAction(Instigator))
			{
				FString InstigatorName = Instigator ? Instigator->GetName() : "None";
				UE_LOG(LogTemp, Warning, TEXT("Failed to start action %s for %s"), *ActionName.ToString(), *InstigatorName);
				continue;
			}
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UCActionComponent::StopActionByTag(AActor* Instigator, FName ActionName)
{
	for (UCAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
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