// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Death.generated.h"

/**
 * Action that handles the ragdoll death
 */
UCLASS()
class CLARITY_API UCAction_Death : public UCAction
{
	GENERATED_BODY()
	
protected:
	void StartAction_Implementation(AActor* Instigator) override;
};
