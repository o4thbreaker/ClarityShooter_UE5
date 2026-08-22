// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_AimCamera.h"
#include "ActionSystem/CActionComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UCAction_AimCamera::UCAction_AimCamera()
{
	bWantsTick = true;

	AimPlayRate = 6.0f;
	TargetFOV = 35.0f;
	TargetArmLength = 135.0f;
	TargetSocketOffset = FVector(0.0f, 35.0f, 45.0f);
}

void UCAction_AimCamera::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	OwnerCharacter = Cast<ACharacter>(NewActionComponent->GetOwner());

	OwnerCamera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	OwnerCameraBoom = OwnerCharacter->FindComponentByClass<USpringArmComponent>();

	// bind the Update pin function
	ProgressDelegate.BindUFunction(this, FName("HandleTimelineProgress"));

	// add the curve float track
	if (MovementCurve)
	{
		AimTimeline.AddInterpFloat(MovementCurve, ProgressDelegate, FName("Alpha"));
	}
}

bool UCAction_AimCamera::CanStartAction_Implementation(AActor* Instigator)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_AimCamera: OwnerCharacter is null"));
		return false;
	}
	
	if (!MovementCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_AimCamera: MovementCurve is null"));
		return false;
	}

	if (!OwnerCamera || !OwnerCameraBoom)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_AimCamera: either CameraComponent or SpringArmComponent are null"));
		return false;
	}

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_AimCamera::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	AimTimeline.SetPlayRate(AimPlayRate);

	AimTimeline.Play();
}

void UCAction_AimCamera::TickAction(float DeltaTime)
{
	Super::TickAction(DeltaTime);
	AimTimeline.TickTimeline(DeltaTime);

	// finish stopping when done playing (while stopping ofc)
	if (bIsStopping && !AimTimeline.IsPlaying())
	{
		bIsStopping = false;
	}
}

void UCAction_AimCamera::HandleTimelineProgress(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Aim Progress: %f"), Value);
	SetCameraOnAim(Value);
}

void UCAction_AimCamera::SetCameraOnAim(float CurrentTime)
{
	OwnerCamera->SetFieldOfView(FMath::Lerp(75.0f, TargetFOV, MovementCurve->GetFloatValue(CurrentTime)));

	OwnerCameraBoom->TargetArmLength = FMath::Lerp(125.0f, TargetArmLength, MovementCurve->GetFloatValue(CurrentTime));

	OwnerCameraBoom->SocketOffset = FMath::Lerp(FVector(0.0f, 50.0f, 30.0f), TargetSocketOffset, MovementCurve->GetFloatValue(CurrentTime));
}

void UCAction_AimCamera::StopAction_Implementation(AActor* Instigator)
{
	AimTimeline.Reverse();
	bIsStopping = true;

	Super::StopAction_Implementation(Instigator);
}