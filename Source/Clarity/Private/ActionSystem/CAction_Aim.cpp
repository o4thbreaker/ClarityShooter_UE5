// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Aim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionSystem/CActionComponent.h"
#include "CGameplayTags.h"
#include "ActionSystem/CAction_AimCamera.h"

UCAction_Aim::UCAction_Aim()
{
	AimingWalkingSpeed = 200.f;
	BufferSpeed = 0.f;
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

	ACharacter* Character = GetActionOwner<ACharacter>();

	if (ensure(Character))
	{
		BufferSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;

		Character->GetCharacterMovement()->MaxWalkSpeed = AimingWalkingSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;

		// start zooming
		GetOwningComponent()->StartActionByTag(Character, CGameplayTags::AimCameraAction);
	}
}

void UCAction_Aim::StopAction_Implementation(AActor* Instigator)
{
	ACharacter* Character = GetActionOwner<ACharacter>();

	if (ensure(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BufferSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

		// stop zooming
		GetOwningComponent()->StopActionByTag(Character, CGameplayTags::AimCameraAction);
	}
	
	Super::StopAction_Implementation(Instigator);
}