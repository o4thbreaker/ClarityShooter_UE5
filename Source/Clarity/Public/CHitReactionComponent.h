// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHitReactionComponent.generated.h"

class USkeletalMeshComponent;
class UPhysicalAnimationComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLARITY_API UCHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	FName CoreBodyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitReaction")
	FName ProfileName;

	UCHitReactionComponent();
	
	UFUNCTION(BlueprintCallable, Category = "HitReaction")
	void PerformHitReaction(const FHitResult& Hit, const float KnockbackTime, const float KnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "HitReaction")
	void HandleDeath();

	FORCEINLINE UPhysicalAnimationComponent* GetPhysicalAnimationComponent() { return PhysicalAnimationComponent; }

protected:
	UPROPERTY(BlueprintReadonly, Category = "Components")
	USkeletalMeshComponent* OwnerMeshComponent;

	UPROPERTY(BlueprintReadonly, Category = "Components")
	UPhysicalAnimationComponent* PhysicalAnimationComponent;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float HitReactionTimeRemaining;
};
