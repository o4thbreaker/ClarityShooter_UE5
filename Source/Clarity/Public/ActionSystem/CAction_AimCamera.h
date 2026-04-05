// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction_Aim.h"
#include "CAction_AimCamera.generated.h"

class ACPlayerCharacter;

/**
 * Action that handles aiming PLAYER's camera
 */
UCLASS()
class CLARITY_API UCAction_AimCamera : public UCAction_Aim
{
	GENERATED_BODY()
	
public:
	UCAction_AimCamera();

protected:

	/* caches in CanStart */
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	ACPlayerCharacter* OwnerPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraDefaultFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingSpeed;

	/* FOV setting for current */
	float CameraCurrentFOV;

	/* timer to handle zooming */
	FTimerHandle AimingTimerHandle;

	/* interpolates the FOV */
	void UpdateFOV();

	/* interpolates the FOV */
	void ResetFOV();

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
};
