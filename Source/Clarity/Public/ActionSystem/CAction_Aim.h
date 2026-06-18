// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Aim.generated.h"

class UCAction_ZoomCamera;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Aim : public UCAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraZoomedFOV;

	/* in/out speed (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraZoomSpeed;

	UCAction_Aim();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AimingWalkingSpeed;

	/* needs to hold the default walking speed */
	float BufferSpeed;

	UPROPERTY()
	UCAction_ZoomCamera* ZoomAction;
};
