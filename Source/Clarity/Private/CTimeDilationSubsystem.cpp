// Fill out your copyright notice in the Description page of Project Settings.


#include "CTimeDilationSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UCTimeDilationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCTimeDilationSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCTimeDilationSubsystem::SetSlowMotion(float SlowMotionTime)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SlowMotionTime);
}

void UCTimeDilationSubsystem::ResetSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
