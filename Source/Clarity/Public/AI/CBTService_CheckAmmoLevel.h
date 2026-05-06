// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckAmmoLevel.generated.h"

class ACWeaponBase;
class UBlackboardComponent;

/**
 * Class that checks the ammo level of the AI's current weapon and updates a LowAmmoKey accordingly. 
 */
UCLASS()
class CLARITY_API UCBTService_CheckAmmoLevel : public UBTService
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LowAmmoPercentage;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowAmmoKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
