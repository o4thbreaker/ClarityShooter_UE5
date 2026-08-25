// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_HitReaction.h"
#include "ActionSystem/CActionComponent.h"
#include "CHitReactionComponent.h"
#include "CAttributeComponent.h"

void UCAction_HitReaction::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	HitReactionComponent = GetActionOwner()->FindComponentByClass<UCHitReactionComponent>();
}

bool UCAction_HitReaction::CanStartAction_Implementation(AActor* Instigator)
{
	if (!ensure(HitReactionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_HitReaction: HitReactionComponent is null"));
		return false;
	}

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_HitReaction::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// get the event data that (hopefully) was passed to this action via TryStartActionWithContext
	const FActionEventData& Data = GetActionEventData();

	// if the event data is valid and contains the expected structure, we can extract it
	if (Data.EventContext.IsValid() && Data.EventContext.GetScriptStruct() == FHealthChangeInfo::StaticStruct())
	{
		// get the const reference without copying the data
		const FHealthChangeInfo& HealthChangeInfo = Data.EventContext.Get<FHealthChangeInfo>();

		HitReactionComponent->PerformHitReaction(HealthChangeInfo.Hit, HealthChangeInfo.KnockbackTime, HealthChangeInfo.KnockbackForce);

		FTimerDelegate StopDelegate;
		StopDelegate.BindUObject(this, &UCAction_HitReaction::EndHitReaction);

		GetWorld()->GetTimerManager().SetTimer(HitReactionTimerHandle, StopDelegate, HealthChangeInfo.KnockbackTime, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_HitReaction: EventContext is invalid or not of type FHealthChangeInfo"));
		StopAction(GetActionOwner());
	}
}

void UCAction_HitReaction::EndHitReaction()
{
	StopAction(GetActionOwner());
}
