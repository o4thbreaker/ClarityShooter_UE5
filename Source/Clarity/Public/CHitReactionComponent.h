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
	
	FORCEINLINE UPhysicalAnimationComponent* GetPhysicalAnimationComponent() { return PhysicalAnimationComponent; }


protected:
	UPROPERTY(BlueprintReadonly, Category = "Components")
	USkeletalMeshComponent* OwnerMeshComponent;

	UPROPERTY(BlueprintReadonly, Category = "Components")
	UPhysicalAnimationComponent* PhysicalAnimationComponent;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "HitReaction")
	void PerformHitReaction(FHitResult Hit, float KnockbackTime, float KnockbackForce);

	UFUNCTION()
	void OnOwnerDamaged(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float NewHealth, FHealthChangeInfo HealthChangeInfo);

private:
	float HitReactionTimeRemaining;
};
