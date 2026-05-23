// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "CBTTask_FindStrafeLocation.generated.h"

class ACAIController;

/**
 * Task to filter the locations from EQS
 */
UCLASS()
class CLARITY_API UCBTTask_FindStrafeLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_FindStrafeLocation();

protected:
	/* EQS for strafe */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	UEnvQuery* LocationSeekerQuery;

	FEnvQueryRequest LocationSeekerQueryRequest;

	UPROPERTY()
	ACAIController* AIController;

	/* Location where AI will be moved */
	FVector StrafeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float DistanceToTarget;

	/* Function will be called when the query is finished */
	void LocationSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	/* Function to select location that is further than specified Location */
	bool IsDistanceGreater(FVector Location);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
