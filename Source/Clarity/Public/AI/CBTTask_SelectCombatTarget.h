// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_SelectCombatTarget.generated.h"

class ACAIController;

/**
 * Task to select the best enemy
 */
UCLASS()
class CLARITY_API UCBTTask_SelectCombatTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_SelectCombatTarget(); 

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EQS")
	UEnvQuery* EnemySeekerQuery;

	FEnvQueryRequest EnemySeekerQueryRequest;

	ACAIController* AIController;
	AActor* BestTarget;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/* Function that gets called when Query is finished*/
	void EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

};
