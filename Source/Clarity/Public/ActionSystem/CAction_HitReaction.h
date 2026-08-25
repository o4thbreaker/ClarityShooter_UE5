// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_HitReaction.generated.h"

class UCHitReactionComponent;

/**
 * Action that handles hit reactions for an actor. This action is triggered when the actor takes damage and is responsible for initiating the appropriate hit reaction behavior, such as playing animations or applying knockback effects.
 */
UCLASS()
class CLARITY_API UCAction_HitReaction : public UCAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	UCHitReactionComponent* HitReactionComponent;

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	bool CanStartAction_Implementation(AActor* Instigator) override;
	void StartAction_Implementation(AActor* Instigator) override;

private:
	FTimerHandle HitReactionTimerHandle;

	UFUNCTION()
	void EndHitReaction();
};
