// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_GetCoverSpot.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_GetCoverSpot : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTask_GetCoverSpot();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Blackboard")
	float DesiredAngleToTarget;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
