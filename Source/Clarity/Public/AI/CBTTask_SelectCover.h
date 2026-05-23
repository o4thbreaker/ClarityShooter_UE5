// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_SelectCover.generated.h"

class ACAIController;
class ACCoverActor;

UENUM(BlueprintType)
enum class ECCoverType : uint8
{
	Approach		UMETA(DisplayName = "Approach"),
	Reposition		UMETA(DisplayName = "Reposition"),
	Retreat			UMETA(DisplayName = "Retreat"),
};

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_SelectCover : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTask_SelectCover();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float DesiredDirectness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	float DesiredDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	ECCoverType RunMode;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Cover")
	UEnvQuery* CoverSeekerQuery;

	FEnvQueryRequest CoverSeekerQueryRequest;

	ACAIController* AIController;

	/* output of the task */
	ACCoverActor* SelectedCover;

	void CoverSeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
