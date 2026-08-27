// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction.h"
#include "ActionSystem/CActionComponent.h"

UCAction::UCAction()
{
	bWantsTick = false;
	bIsAutoStart = false;
}

void UCAction::Initialize(UCActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

void UCAction::StartActionWithContext(AActor* Instigator, const FActionEventData& EventData)
{
	ActionData = EventData;

	StartAction(Instigator);
}

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));

	UCActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	NotifyTagsStateChanged(GrantsTags, true);

	bIsRunning = true;

	Component->OnActionStarted.Broadcast(Component, this);
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));

	UCActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	NotifyTagsStateChanged(GrantsTags, false);

	bIsRunning = false;

	Component->OnActionStopped.Broadcast(Component, this);
}

bool UCAction::CanStartAction_Implementation(AActor* Instigator)
{
	// if action is already running
	if (bIsRunning)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't start %s because it is already running."), *GetNameSafe(this));
		return false;
	}

	UCActionComponent* Component = GetOwningComponent();

	// if owner has any of blocked tags
	if (Component->ActiveGameplayTags.HasAny(BlockedTags))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't start %s because owner has one or more of blocked tags."), *GetNameSafe(this));
		return false;
	}
		
	// otherwise
	return true;
}

AActor* UCAction::GetActionOwner() const
{
	return GetOwningComponent() ? GetOwningComponent()->GetOwner() : nullptr; 
}

UWorld* UCAction::GetWorld() const
{
	//thing we instantiated in ActionComponent when created new Action
	//basically it is ActionComponent itself
	AActor* Actor = Cast<AActor>(GetOuter());

	if (Actor) { return Actor->GetWorld(); }

	return nullptr;
}

void UCAction::NotifyTagsStateChanged(FGameplayTagContainer TagsToNotify, bool bIsStarted)
{
	// if needed to notify about ACTION TAG itself
	//TagsToNotify.AddTag(ActionTag);

	// notify about adding or notify about removing the tags
	bIsStarted ? GetOwningComponent()->NotifyTagsAdded(TagsToNotify) : GetOwningComponent()->NotifyTagsRemoved(TagsToNotify);
}
