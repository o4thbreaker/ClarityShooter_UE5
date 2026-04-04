// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction.h"
#include "ActionSystem/CActionComponent.h"

void UCAction::Initialize(UCActionComponent* NewActionComponent)
{
	ActionComponent = NewActionComponent;
}

void UCAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StartAction: %s"), *GetNameSafe(this));

	UCActionComponent* Component = GetOwningComponent();
	
	Component->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;

	Component->OnActionStarted.Broadcast(Component, this);
}

void UCAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("StopAction: %s"), *GetNameSafe(this));

	UCActionComponent* Component = GetOwningComponent();

	Component->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;

	Component->OnActionStopped.Broadcast(Component, this);
}

bool UCAction::CanStartAction_Implementation(AActor* Instigator)
{
	// if action is already running
	if (bIsRunning) return false;

	UCActionComponent* Component = GetOwningComponent();

	// if owner has any of blocked tags
	if (Component->ActiveGameplayTags.HasAny(BlockedTags)) return false;

	// otherwise
	return true;
}

UWorld* UCAction::GetWorld() const
{
	//thing we instantiated in ActionComponent when created new Action
	//basically it is ActionComponent itself
	AActor* Actor = Cast<AActor>(GetOuter());

	if (Actor) { return Actor->GetWorld(); }

	return nullptr;
}