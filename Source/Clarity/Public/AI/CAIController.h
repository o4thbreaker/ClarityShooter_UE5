// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
struct FAIStimulus;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class ACAICharacter;
class ACAIManager;

/**
 * 
 */
UCLASS()
class CLARITY_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIManager* AIManager;

	ACAIController();

	UFUNCTION()
	void OnPerception(AActor* SpottedActor, FAIStimulus Stimulus);

	/* Sets TargetActor key */
	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	/* Sets ShootFromCover key */
	UFUNCTION(BlueprintCallable)
	void SetShouldShootFromCover(bool ShouldShoot);

	FORCEINLINE AActor* GetTargetActor() const;
	FORCEINLINE float GetStimulusTimeStamp() const { return StimulusTimeStamp; };
	FORCEINLINE ACAICharacter* GetAICharacter() const { return AICharacter; };

protected:
	UPROPERTY(BlueprintReadonly, Category = "AI")
	UBehaviorTreeComponent* BTComponent;
	
	UPROPERTY(BlueprintReadonly, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(BlueprintReadonly, Category = "AI")
	ACAICharacter* AICharacter;

	/* Variable to set the threshold before AI start to detect */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectionThreshold;

	/* Variable to control speed of detection */
	float DetectionLevel;

	/* Variable to track the time stamp of the last stimulus received */
	float StimulusTimeStamp;

	FTimerHandle DetectionTimer;

	/* Controls the rate of detection */
	float DetectionRate;

	/* The location of the last stimulus received */
	FVector LastStimulusLocation;
	
	/* Who was detected by sense*/
	AActor* Target;

	/* Sight Sense config */
	UAISenseConfig_Sight* Sight;

	void SetDetectionLevel();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
};
