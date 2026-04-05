// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Aim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAction_Aim::UCAction_Aim()
{
	DefaultWalkingSpeed = 300.f;
	AimingWalkingSpeed = 200.f;
}

void UCAction_Aim::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (ensure(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = AimingWalkingSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void UCAction_Aim::StopAction_Implementation(AActor* Instigator)
{
	ACharacter* Character = Cast<ACharacter>(Instigator);

	if (ensure(Character))
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = DefaultWalkingSpeed;
		Character->GetCharacterMovement()->bOrientRotationToMovement = true;
		Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
	
	Super::StopAction_Implementation(Instigator);
}

