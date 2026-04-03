// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	
	/* recoil to achieve (might be 0 when we're not shooting) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
	FTransform TargetRecoil;
	
	/* recoil at the moment */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shoot")
	FTransform ActualRecoil;

	/* how fast the recoil fades off */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float RecoilRecoverySpeed;

	/* constructor */
	void NativeInitializeAnimation() override;

	/* tick for the animation */
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	/* method to handle recoil */
	UFUNCTION(BlueprintCallable, Category = "Shoot")
	void DoProceduralRecoil(float RecoilMultiplier);
};
