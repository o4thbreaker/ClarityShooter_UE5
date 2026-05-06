// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_Reload.generated.h"

class APawn;
class UCActionComponent;
class UCAction;
class UBehaviorTreeComponent;

USTRUCT()
struct FReloadDataMemory
{
	GENERATED_BODY()
	UCActionComponent* OwnerActionComponent = nullptr;
};

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTask_Reload : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_Reload();

protected:	
	UFUNCTION()
	void OnReloadFinished(UCActionComponent* ActionComponent, UCAction* Action);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	uint16 GetInstanceMemorySize() const override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

};
