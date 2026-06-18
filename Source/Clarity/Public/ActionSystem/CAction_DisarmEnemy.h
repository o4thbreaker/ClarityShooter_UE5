// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_DisarmEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_DisarmEnemy : public UCAction
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Target;

	UCAction_DisarmEnemy();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Range")
	float Range;

	UFUNCTION()
	FHitResult GetTraceHitInfo(AActor* FromActor);
};
