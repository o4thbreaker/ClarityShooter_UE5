// Fill out your copyright notice in the Description page of Project Settings.


#include "CCameraModifier_Aim.h"

UCCameraModifier_Aim::UCCameraModifier_Aim()
{
	CameraDefaultFOV = 90.0f;
	CameraAimingFOV = 40.0f;
}

void UCCameraModifier_Aim::InitializeFOVData(float NewDefaultFOV, float NewAimingFOV, float NewAimingSpeed)
{
	CameraDefaultFOV = NewDefaultFOV;
	CameraAimingFOV = NewAimingFOV;
	AlphaInTime = NewAimingSpeed;
	AlphaOutTime = NewAimingSpeed;
}

bool UCCameraModifier_Aim::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);

	InOutPOV.FOV = CameraDefaultFOV + Alpha * (CameraAimingFOV - CameraDefaultFOV);

	return false;
}
