// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
struct FAIStimulus;

/**
 * 
 */
UCLASS()
class CLARITY_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:

	ACAIController();

	UFUNCTION()
	void OnPerception(AActor* SpottedActor, FAIStimulus Stimulus);

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	FORCEINLINE AActor* GetCurrentTarget() const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;
	
	/* Sight Sense config */
	UAISenseConfig_Sight* Sight;

	virtual void OnPossess(APawn* InPawn) override;
};
