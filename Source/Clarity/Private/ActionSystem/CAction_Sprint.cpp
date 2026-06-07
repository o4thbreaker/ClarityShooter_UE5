// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Sprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "ActionSystem/CAction_ZoomCamera.h"
#include "CGameplayTags.h"

UCAction_Sprint::UCAction_Sprint()
{
	SprintSpeed = 600.0f;
	BufferSpeed = 0.0f;
	CameraZoomedFOV = 60.0f;
	CameraZoomSpeed = 0.2f;
}

void UCAction_Sprint::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	// initialize zoom action
	ZoomAction = Cast<UCAction_ZoomCamera>(NewActionComponent->AddAction(NewActionComponent->GetOwner(), UCAction_ZoomCamera::StaticClass()));
	ZoomAction->ActionTag = CGameplayTags::SprintCameraAction;
	ZoomAction->CameraZoomedFOV = CameraZoomedFOV;
	ZoomAction->CameraZoomSpeed = CameraZoomSpeed;
}

void UCAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Owner = Cast<ACharacter>(Instigator);
	if (Owner)
	{
		// store original speed
		BufferSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;

		// accelerate
		Owner->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		GetOwningComponent()->StartActionByTag(Instigator, CGameplayTags::SprintCameraAction);
	}
}

void UCAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	ACharacter* Owner = Cast<ACharacter>(Instigator);
	if (Owner)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = BufferSpeed;

		GetOwningComponent()->StopActionByTag(Instigator, CGameplayTags::SprintCameraAction);
	}

	Super::StopAction_Implementation(Instigator);
}
