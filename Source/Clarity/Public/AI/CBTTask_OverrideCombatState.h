// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Clarity.h"
#include "CBTTask_OverrideCombatState.generated.h"

/**
 * Task to easily switch between combat states
 */
UCLASS()
class CLARITY_API UCBTTask_OverrideCombatState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_OverrideCombatState();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	ECCombatState DesiredState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
