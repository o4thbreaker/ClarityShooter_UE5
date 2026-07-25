// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clarity.h"
#include "Subsystems/WorldSubsystem.h"
#include "CAIManagerSubsystem.generated.h"

class ACAIController;
class ACAICharacter;

/**
 * Managers that coordinates the AI of the same team. Don't forget to inherit.
 */
UCLASS(Abstract, Blueprintable)
class CLARITY_API UCAIManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
	/// \NOTE: do not forget the BP engine bug!
	///		   when adding new BP Manager ALWAYS add it to the asset manager in project settings

public:
	/* Agent that should provide cover fire for others */
	ACAICharacter* AgentProvidingCoverFire;

	/* One of the defenders that is currently repositioning */
	int32 ApproacherIndex;

	UCAIManagerSubsystem();

	UFUNCTION()
	void CreateAgentsList();

	/* Sets the AI state to the all the agents in array */
	UFUNCTION()
	void NotifyAIState(ECAIState NewState);

	UFUNCTION()
	void RemoveAgent(ACAIController* AgentToRemove);

	UFUNCTION(BlueprintCallable)
	void RequestCoverFire(bool ShouldProvideCoverFire, ACAICharacter* RequestInstigator);

	FORCEINLINE const TArray<ACAIController*> GetAgents() const { return Agents; }
	FORCEINLINE const TArray<ACAICharacter*> GetDefenders() const { return Defenders; }

protected:
	/* array of all agents that have the same faction as this manager*/
	UPROPERTY(BlueprintReadonly, Category = "AI")
	TArray<ACAIController*> Agents;

	/* Array of agents that hold cover or repositioning */
	UPROPERTY(BlueprintReadonly, Category = "AI")
	TArray<ACAICharacter*> Defenders;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	ECFaction ManagerFaction;

	/* Variable to set how long should Combat Loop go if there are now valid targets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxStimulusTime_Combat;

	/* Variable to set how long should Search Loop go if there are now valid targets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxStimulusTime_Search;

	FTimerHandle CombatTimer;
	FTimerHandle SearchTimer;

	/* time when Search behaviour has been started */
	float SearchTimeStamp;

	/* The base search loop of units */
	UFUNCTION()
	void RunSearchTimer();

	/* the base combat loop of units */
	UFUNCTION()
	void RunCombatLoop();

	/* are any of the agents has line of sight to an enemy or if the time from the last stimulus > max stimulus time? */
	UFUNCTION()
	bool IsEngaged();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};
