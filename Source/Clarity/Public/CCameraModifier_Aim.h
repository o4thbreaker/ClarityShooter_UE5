// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CCameraModifier_Aim.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCCameraModifier_Aim : public UCameraModifier
{
	GENERATED_BODY()

public:
	UCCameraModifier_Aim();

	void InitializeFOVData(float NewDefaultFOV, float NewAimingFOV, float NewAimingSpeed);

protected:
	float CameraDefaultFOV;
	float CameraAimingFOV;

	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
	
};
