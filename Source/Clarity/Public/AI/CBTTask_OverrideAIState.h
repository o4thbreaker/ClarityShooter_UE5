// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clarity.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_OverrideAIState.generated.h"

UENUM(BlueprintType)
enum class ECRunMode : uint8
{
	ThisAgent		UMETA(DisplayName = "This Agent"),
	AllAgents		UMETA(DisplayName = "All Agents")
};

/**
 * Task to change the AI state to the desired one
 */
UCLASS()
class CLARITY_API UCBTTask_OverrideAIState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_OverrideAIState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	ECAIState DesiredState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	ECRunMode RunMode;

};
