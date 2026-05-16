// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_AimCamera.h"
#include "CPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "ActionSystem/CActionComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "CCameraModifier_Aim.h"

UCAction_AimCamera::UCAction_AimCamera()
{
	CameraDefaultFOV = 90.0f;
	CameraAimingFOV = 40.0f;
	CameraAimingSpeed = 0.3f;
}

void UCAction_AimCamera::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	/// \WARNING: may cause issues because of Inititalize gets called on BeginPlay 
	///           and PlayerController might be null by that point

	OwnerPlayerCharacter = Cast<ACPlayerCharacter>(NewActionComponent->GetOwner());

	CameraDefaultFOV = OwnerPlayerCharacter->GetFollowCamera()->FieldOfView;

	APlayerController* PlayerController = Cast<APlayerController>(OwnerPlayerCharacter->GetController());
	if (PlayerController)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
		if (!Modifier)
		{
			Modifier = Cast<UCCameraModifier_Aim>(CameraManager->AddNewCameraModifier(UCCameraModifier_Aim::StaticClass()));
		}
		Modifier->InitializeFOVData(CameraDefaultFOV, CameraAimingFOV, CameraAimingSpeed);
		Modifier->DisableModifier();
	}
}

bool UCAction_AimCamera::CanStartAction_Implementation(AActor* Instigator)
{
	if (!Modifier) return false;

	return Super::CanStartAction_Implementation(Instigator) && ensureAlways(OwnerPlayerCharacter);
}

void UCAction_AimCamera::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	Modifier->EnableModifier();
}

void UCAction_AimCamera::StopAction_Implementation(AActor* Instigator)
{
	if (Modifier)
	{
		Modifier->DisableModifier();
	}

	Super::StopAction_Implementation(Instigator);
}