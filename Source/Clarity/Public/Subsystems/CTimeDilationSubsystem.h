// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CTimeDilationSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCTimeDilationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Time Dilation")
	void SetSlowMotion(float NewSlowMotion);

	UFUNCTION(BlueprintCallable, Category = "Time Dilation")
	void ResetSlowMotion();
};
