// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_Sprint.h"
#include "ActionSystem/CActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAction_Sprint::UCAction_Sprint()
{
	SprintSpeed = 500.0f;
	SprintInterpolationTime = 5.0f;
}

void UCAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	Owner = Cast<ACharacter>(Instigator);

	TargetSpeed = SprintSpeed;

	if (!GetWorld()->GetTimerManager().IsTimerActive(SprintTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &UCAction_Sprint::Accelerate, GetWorld()->GetDeltaSeconds(), true);
	}
}

void UCAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	TargetSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
	
	Super::StopAction_Implementation(Instigator);
}

void UCAction_Sprint::Accelerate()
{
	float CurrentSpeed = Owner->GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, GetWorld()->GetDeltaSeconds(), SprintInterpolationTime);

	Owner->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

	if (FMath::IsNearlyEqual(NewSpeed, TargetSpeed, 1.f))
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	}
}
