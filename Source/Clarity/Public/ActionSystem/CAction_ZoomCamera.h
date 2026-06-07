// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_ZoomCamera.generated.h"

class ACPlayerCharacter;
class UCCameraModifier_Zoom;
class APlayerCameraManager;

/**
 * Action to zoom camera based on lerp equation
 */
UCLASS()
class CLARITY_API UCAction_ZoomCamera : public UCAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraZoomSpeed;

	/* in/out speed (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraZoomedFOV;

	UCAction_ZoomCamera();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	ACPlayerCharacter* OwnerPlayerCharacter;

	float CameraCurrentFOV;

private:
	UPROPERTY()
	UCCameraModifier_Zoom* Modifier;

	UPROPERTY()
	APlayerCameraManager* CameraManager;
	
};
