// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector InAttackRangeKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 2000.f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
