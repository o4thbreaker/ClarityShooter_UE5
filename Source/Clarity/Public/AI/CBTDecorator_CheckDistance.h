// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CBTDecorator_CheckDistance.generated.h"

/**
 * Checks if the distance between this actor and the target is less than X
 */
UCLASS()
class CLARITY_API UCBTDecorator_CheckDistance : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UCBTDecorator_CheckDistance();

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	float DistanceToCheck;

	/** calculates raw, core value of decorator's condition. Should not include calling IsInversed */
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
