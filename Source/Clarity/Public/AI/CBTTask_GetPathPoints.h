// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_GetPathPoints.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_GetPathPoints : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_GetPathPoints(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 Index;
};
