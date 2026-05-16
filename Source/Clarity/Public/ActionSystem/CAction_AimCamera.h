// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction_Aim.h"
#include "CAction_AimCamera.generated.h"

class ACPlayerCharacter;
class UCCameraModifier_Aim;

/**
 * Action that handles aiming PLAYER's camera
 */
UCLASS()
class CLARITY_API UCAction_AimCamera : public UCAction_Aim
{
	GENERATED_BODY()
	
public:
	UCAction_AimCamera();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	ACPlayerCharacter* OwnerPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraDefaultFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingFOV;

	/* in/out speed (in seconds) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraAimingSpeed;

private:
	UCCameraModifier_Aim* Modifier;
};
