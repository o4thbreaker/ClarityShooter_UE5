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
class UCAIManagerSubsystem;
class UCWeaponSlotsComponent;

/**
 * 
 */
UCLASS()
class CLARITY_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	UCAIManagerSubsystem* AIManager;

	ACAIController();

	UFUNCTION()
	void OnPerception(AActor* SpottedActor, FAIStimulus Stimulus);

	/* Sets TargetActor key */
	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI|Roles")
	bool TrySetAIState(ECAIState NewState);

	UFUNCTION(BlueprintCallable, Category = "AI|Roles")
	uint8 GetAIState() const;

	UFUNCTION(BlueprintCallable, Category = "AI|Roles")
	void SetCombatState(ECCombatState NewState);

	UFUNCTION(BlueprintCallable, Category = "AI|Roles")
	uint8 GetCombatState() const;

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

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnDamaged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, FHealthChangeInfo HealthChangeInfo);

	UFUNCTION()
	void OnStopHitReaction(bool IsStarted);

	UFUNCTION(BlueprintCallable)
	void OnWeaponLost(UCWeaponSlotsComponent* OwningComponent);
};
