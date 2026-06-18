// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Aim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "ActionSystem/CAction_ZoomCamera.h"

UCAction_Aim::UCAction_Aim()
{
	AimingWalkingSpeed = 200.f;
	BufferSpeed = 0.f;
	CameraZoomedFOV = 40.0f;
	CameraZoomSpeed = 0.3f;
}

void UCAction_Aim::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	// initialize zoom action
	ZoomAction = Cast<UCAction_ZoomCamera>(NewActionComponent->AddAction(NewActionComponent->GetOwner(), UCAction_ZoomCamera::StaticClass()));
	ZoomAction->ActionTag = CGameplayTags::AimCameraAction;
	ZoomAction->CameraZoomedFOV = CameraZoomedFOV;
	ZoomAction->CameraZoomSpeed = CameraZoomSpeed;
}

bool UCAction_Aim::CanStartAction_Implementation(AActor* Instigator)
{
	if (!GetOwningComponent()->ActiveGameplayTags.HasTag(CGameplayTags::Armed))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't start Aiming because character is not armed"));
		return false;
	}

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_Aim::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// disable sprinting
	if (GetOwningComponent()->ActiveGameplayTags.HasTag(CGameplayTags::Sprinting))
	{
		GetOwningComponent()->StopActionByTag(Instigator, CGameplayTags::SprintAction);
	}

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (ensure(Character))
	{
		BufferSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

		Character->GetCharacterMovement()->MaxWalkSpeed = AimingWalkingSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;

		// start zooming
		GetOwningComponent()->StartActionByTag(Instigator, CGameplayTags::AimCameraAction);
	}
}

void UCAction_Aim::StopAction_Implementation(AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (ensure(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BufferSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

		// stop zooming
		GetOwningComponent()->StopActionByTag(Instigator, CGameplayTags::AimCameraAction);
	}
	
	Super::StopAction_Implementation(Instigator);
}

