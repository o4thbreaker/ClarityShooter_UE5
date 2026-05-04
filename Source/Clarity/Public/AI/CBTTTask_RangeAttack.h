// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTTask_RangeAttack.generated.h"

class ACAICharacter;
class UCActionComponent;
class UCWeaponSlotsComponent;

USTRUCT()
struct FRangeAttackDataMemory
{
	GENERATED_BODY()

	ACAICharacter* Owner;
	UCActionComponent* OwnerActionComponent;
	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent;
};

/**
 * 
 */
UCLASS()
class CLARITY_API UCBTTTask_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTTask_RangeAttack();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float MagazineThresholdPercentage;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	uint16 GetInstanceMemorySize() const override;

private:
	bool IsMemoryValid(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FRangeAttackDataMemory* Memory);
};
