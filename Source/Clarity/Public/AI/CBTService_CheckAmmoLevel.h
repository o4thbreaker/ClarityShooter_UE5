// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckAmmoLevel.generated.h"

class ACWeaponBase;
class UBlackboardComponent;
class UCBTAmmoObserver;

USTRUCT()
struct FCheckAmmoLevelMemory
{
	GENERATED_BODY()

	UCBTAmmoObserver* Observer = nullptr;
};

/**
 * Class that checks the ammo level of the AI's current weapon and updates a LowAmmoKey accordingly. 
 * It uses an observer pattern to listen for changes in the ammo count.
 */
UCLASS()
class CLARITY_API UCBTService_CheckAmmoLevel : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_CheckAmmoLevel();

protected:

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float LowAmmoPercentage;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowAmmoKey;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	uint16 GetInstanceMemorySize() const override;

	/* function from UObject that adds non-UPROPERTY objects to UE's GC */
	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);
};
