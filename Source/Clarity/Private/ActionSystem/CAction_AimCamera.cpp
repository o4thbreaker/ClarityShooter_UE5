// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_AimCamera.h"
#include "CPlayerCharacter.h"
#include "Camera/CameraComponent.h"

UCAction_AimCamera::UCAction_AimCamera()
{
	CameraDefaultFOV = 90.0f;
	CameraCurrentFOV = CameraDefaultFOV;
	CameraAimingFOV = 40.0f;
	CameraAimingSpeed = 20.0f;
}

bool UCAction_AimCamera::CanStartAction_Implementation(AActor* Instigator)
{
	// if owner is not valid or not player character, return false
	OwnerPlayerCharacter = Cast<ACPlayerCharacter>(Instigator);

	return Super::CanStartAction_Implementation(Instigator) && ensureAlways(OwnerPlayerCharacter);
}

void UCAction_AimCamera::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, this, &UCAction_AimCamera::UpdateFOV, GetWorld()->GetDeltaSeconds(), true);
}

void UCAction_AimCamera::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(AimingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
	
	GetWorld()->GetTimerManager().SetTimer(AimingTimerHandle, this, &UCAction_AimCamera::ResetFOV, GetWorld()->GetDeltaSeconds(), true);

	Super::StopAction_Implementation(Instigator);
}

void UCAction_AimCamera::UpdateFOV()
{
	if (!IsValid(OwnerPlayerCharacter)) return;

	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraAimingFOV, GetWorld()->GetDeltaSeconds(), CameraAimingSpeed);
	OwnerPlayerCharacter->GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);

	if (FMath::IsNearlyEqual(CameraCurrentFOV, CameraAimingFOV, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
}

void UCAction_AimCamera::ResetFOV()
{
	if (!IsValid(OwnerPlayerCharacter)) return;

	CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, GetWorld()->GetDeltaSeconds(), CameraAimingSpeed);
	OwnerPlayerCharacter->GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);

	if (FMath::IsNearlyEqual(CameraCurrentFOV, CameraDefaultFOV, 0.1f))
	{
		GetWorld()->GetTimerManager().ClearTimer(AimingTimerHandle);
	}
}