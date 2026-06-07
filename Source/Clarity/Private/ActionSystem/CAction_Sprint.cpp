// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Sprint.h"
#include "ActionSystem/CActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAction_Sprint::UCAction_Sprint()
{
	SprintSpeed = 600.0f;
	BufferSpeed = 0.f;
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
	}
}

void UCAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	ACharacter* Owner = Cast<ACharacter>(Instigator);

	if (Owner)
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = BufferSpeed;
	}

	Super::StopAction_Implementation(Instigator);
}
