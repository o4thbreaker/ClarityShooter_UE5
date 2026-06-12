// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_ZoomCamera.h"
#include "CPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "ActionSystem/CActionComponent.h" 
#include "Camera/PlayerCameraManager.h"
#include "CCameraModifier_Zoom.h"
#include "CGameplayTags.h"

UCAction_ZoomCamera::UCAction_ZoomCamera()
{
	// change when adding to action
	ActionTag = CGameplayTags::AimCameraAction;

	CameraCurrentFOV = 90.0f;
	CameraZoomedFOV = 40.0f;
	CameraZoomSpeed = 0.3f;
}

void UCAction_ZoomCamera::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	/// \WARNING: may cause issues because of Inititalize gets called on BeginPlay 
	///           and PlayerController might be null by that point

	OwnerPlayerCharacter = Cast<ACPlayerCharacter>(NewActionComponent->GetOwner());

	if (OwnerPlayerCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(OwnerPlayerCharacter->GetController());
		if (PlayerController)
		{
			CameraManager = PlayerController->PlayerCameraManager;
			if (!Modifier)
			{
				Modifier = Cast<UCCameraModifier_Zoom>(CameraManager->AddNewCameraModifier(UCCameraModifier_Zoom::StaticClass()));
			}

			Modifier->DisableModifier();
		}
	}
}

bool UCAction_ZoomCamera::CanStartAction_Implementation(AActor* Instigator)
{
	if (!Modifier) return false;

	return Super::CanStartAction_Implementation(Instigator) && ensureAlways(OwnerPlayerCharacter);
}

void UCAction_ZoomCamera::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// get current fov (cant get from FollowCamera, it stores wrong value)
	CameraCurrentFOV = CameraManager->GetFOVAngle();

	Modifier->InitializeFOVData(CameraCurrentFOV, CameraZoomedFOV, CameraZoomSpeed);
	Modifier->EnableModifier();
}

void UCAction_ZoomCamera::StopAction_Implementation(AActor* Instigator)
{
	if (Modifier)
	{
		Modifier->DisableModifier();
	}

	Super::StopAction_Implementation(Instigator);
}