// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/CAction_SwapShoulder.h"
#include "ActionSystem/CActionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "CGameplayTags.h"

UCAction_SwapShoulder::UCAction_SwapShoulder()
{
	bWantsTick = true;
}

void UCAction_SwapShoulder::Initialize(UCActionComponent* NewActionComponent)
{
	Super::Initialize(NewActionComponent);

	OwnerCharacter = GetActionOwner<ACharacter>();

	if (OwnerCharacter)
	{
		OwnerCameraBoom = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
	}

	// bind the Update pin function
	ProgressDelegate.BindUFunction(this, FName("HandleTimelineProgress"));

	// bind the Finished pin function
	FinishedDelegate.BindUFunction(this, FName("HandleTimelineFinished"));

	// add the curve float track
	if (TimelineCurve)
	{
		SwapTimeline.AddInterpFloat(TimelineCurve, ProgressDelegate, FName("Alpha"));
	}

	SwapTimeline.SetTimelineFinishedFunc(FinishedDelegate);
}

bool UCAction_SwapShoulder::CanStartAction_Implementation(AActor* Instigator)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_SwapShoulder: OwnerCharacter is null"));
		return false;
	}

	if (!TimelineCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_SwapShoulder: float curve is null"));
		return false;
	}

	if (!OwnerCameraBoom)
	{
		UE_LOG(LogTemp, Error, TEXT("UCAction_SwapShoulder: SpringArmComponent is null"));
		return false;
	}

	return Super::CanStartAction_Implementation(Instigator);
}

void UCAction_SwapShoulder::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	InitialSocketOffsetY = OwnerCameraBoom->SocketOffset.Y;

	if (!ActionComponent->ActiveGameplayTags.HasTag(CGameplayTags::ShoulderSwapped))
	{
		ActionComponent->ActiveGameplayTags.AddTag(CGameplayTags::ShoulderSwapped);
	}
	else
	{
		ActionComponent->ActiveGameplayTags.RemoveTag(CGameplayTags::ShoulderSwapped);
	}

	SwapTimeline.PlayFromStart();
}

void UCAction_SwapShoulder::TickAction(float DeltaTime)
{
	Super::TickAction(DeltaTime);
	SwapTimeline.TickTimeline(DeltaTime);
}

void UCAction_SwapShoulder::HandleTimelineProgress(float Value)
{
	float NewSocketOffsetY = FMath::Lerp(InitialSocketOffsetY, -InitialSocketOffsetY, Value);
	OwnerCameraBoom->SocketOffset.Y = NewSocketOffsetY;
}

void UCAction_SwapShoulder::HandleTimelineFinished()
{
	StopAction(OwnerCharacter);
}