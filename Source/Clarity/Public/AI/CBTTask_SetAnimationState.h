// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTask_SetAnimationState.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_SetAnimationState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UCBTTask_SetAnimationState();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	bool bInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	bool bShouldFocus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	bool bIsCrouching;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
