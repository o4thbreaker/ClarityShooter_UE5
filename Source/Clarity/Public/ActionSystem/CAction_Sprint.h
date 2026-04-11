// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Sprint : public UCAction
{
	GENERATED_BODY()

public:
	UCAction_Sprint();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint")
	float SprintInterpolationTime;

	UPROPERTY()
	ACharacter* Owner;

	/* value to interpolate to (sprint or default)*/
	float TargetSpeed;

	FTimerHandle SprintTimerHandle;

	UFUNCTION()
	void Accelerate();
};
