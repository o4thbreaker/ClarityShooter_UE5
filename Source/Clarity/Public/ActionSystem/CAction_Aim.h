// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Aim.generated.h"

//struct FTimerHandle;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Aim : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_Aim();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AimingWalkingSpeed;
};
