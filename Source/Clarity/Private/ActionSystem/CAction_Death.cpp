// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Death.h"
#include "CHitReactionComponent.h"

void UCAction_Death::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCHitReactionComponent* HitReactionComponent = GetActionOwner()->FindComponentByClass<UCHitReactionComponent>();

	if (HitReactionComponent)
	{
		HitReactionComponent->HandleDeath();
	}

	// should we stop the action?
	// StopAction(Instigator);
}
