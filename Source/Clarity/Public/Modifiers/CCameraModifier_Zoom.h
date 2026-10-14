// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CCameraModifier_Zoom.generated.h"

/**
 * Modifies camera current fov via lerp equation
 */
UCLASS()
class CLARITY_API UCCameraModifier_Zoom : public UCameraModifier
{
	GENERATED_BODY()

public:
	UCCameraModifier_Zoom();

	void InitializeFOVData(float NewDefaultFOV, float NewAimingFOV, float NewAimingSpeed);

protected:
	float CurrentFOV;
	float TargetFOV;

	virtual bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
	
};
