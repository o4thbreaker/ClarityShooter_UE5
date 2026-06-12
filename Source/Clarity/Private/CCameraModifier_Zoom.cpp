// Fill out your copyright notice in the Description page of Project Settings.


#include "CCameraModifier_Zoom.h"

UCCameraModifier_Zoom::UCCameraModifier_Zoom()
{
	CurrentFOV = 90.0f;
	TargetFOV = 40.0f;
}

void UCCameraModifier_Zoom::InitializeFOVData(float NewFOV, float NewTargetFOV, float ZoomSpeed)
{
	CurrentFOV = NewFOV;
	TargetFOV = NewTargetFOV;
	AlphaInTime = ZoomSpeed;
	AlphaOutTime = ZoomSpeed;
}

bool UCCameraModifier_Zoom::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
	Super::ModifyCamera(DeltaTime, InOutPOV);

	InOutPOV.FOV = CurrentFOV + Alpha * (TargetFOV - CurrentFOV);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, FString::Printf(TEXT("FOV: %.1f"), InOutPOV.FOV));
	//UE_LOG(LogTemp, Log, TEXT("%f"), InOutPOV.FOV);

	return true;
}
