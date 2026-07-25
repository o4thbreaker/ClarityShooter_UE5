// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_DisarmEnemy.generated.h"

class UContextualAnimSceneAsset;

/**
 * Action that plays the Disarm animation that fires the RetrieveWeapon notify
 */
UCLASS()
class CLARITY_API UCAction_DisarmEnemy : public UCAction
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Target;

	UCAction_DisarmEnemy();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	/* stores the disarm CAS */
	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Animation")
	UContextualAnimSceneAsset* ContextualAnimAsset;

	UPROPERTY(BlueprintReadonly, Category = "Animation")
	UContextualAnimSceneActorComponent* AnimSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Properties")
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadonly, Category = "Properties", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SlowMotionTime;

	UFUNCTION()
	FHitResult GetTraceHitInfo(AActor* FromActor);

	UFUNCTION()
	bool PlayContextualAnimation(AActor* Attacker, AActor* Victim);

private:
	UFUNCTION()
	void OnAnimationEnd(UContextualAnimSceneActorComponent* SceneActorComponent);
};
