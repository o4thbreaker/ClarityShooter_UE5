// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clarity.h"
#include "CAIManager.generated.h"

class ACAIController;

UCLASS()
class CLARITY_API ACAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAIManager();

	UFUNCTION()
	void CreateAgentsList();

	/* Sets the AI state to the all the agents in array */
	UFUNCTION()
	void NotifyAIState(ECAIState NewState);

	UFUNCTION()
	void RemoveAgent(ACAIController* AgentToRemove);

protected:

	/* array of all agents that have the same faction as this manager*/
	UPROPERTY(BlueprintReadonly, Category = "AI")
	TArray<ACAIController*> Agents;

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

	/* Time when Search behaviour has been started */
	float SearchTimeStamp;

	UFUNCTION()
	void RunSearchTimer();

	UFUNCTION()
	void RunCombatLoop();

	/* Are any of the agents has line of sight to an enemy or if the time from the last stimulus > max stimulus time? */
	UFUNCTION()
	bool Engaged();

	virtual void BeginPlay() override;

};
