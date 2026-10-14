// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Death.h"
#include "CHitReactionComponent.h"
#include "CAttributeComponent.h"

void UCAction_Death::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCHitReactionComponent* HitReactionComponent = GetActionOwner()->FindComponentByClass<UCHitReactionComponent>();

	// get the event data that (hopefully) was passed to this action via TryStartActionWithContext
	const FActionEventData& Data = GetActionEventData();

	if (HitReactionComponent && Data.EventContext.IsValid() && Data.EventContext.GetScriptStruct() == FHealthChangeInfo::StaticStruct())
	{
		// get the const reference without copying the data
		const FHealthChangeInfo& HealthChangeInfo = Data.EventContext.Get<FHealthChangeInfo>();

		HitReactionComponent->HandleDeath(HealthChangeInfo.Hit, HealthChangeInfo.KnockbackForce);
	}

	// should we stop the action?
	// StopAction(Instigator);
}
