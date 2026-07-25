// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_StartAiming.generated.h"

/**
 * Task thats selects target and starts to aim at it (IS NOT IN USE RN)
 */
UCLASS()
class CLARITY_API UCBTTask_StartAiming : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorKey;

	/// \TODO: refactor to stop calling the same functionality
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
