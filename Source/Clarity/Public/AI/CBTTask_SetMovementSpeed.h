// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Clarity.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTask_SetMovementSpeed();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	ECMovementMode DesiredMovementMode;

	float WalkingSpeed;
	float JoggingSpeed;
	float SprintingSpeed;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
