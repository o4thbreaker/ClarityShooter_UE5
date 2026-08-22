// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "Components/TimelineComponent.h"
#include "CAction_AimCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCurveFloat;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_AimCamera : public UCAction
{
	GENERATED_BODY()
	
public:
	// the Curve is used to control the timeline track
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float TargetFOV;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float TargetArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector TargetSocketOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float AimPlayRate;

	UCAction_AimCamera();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	UCameraComponent* OwnerCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* OwnerCameraBoom;

	// Mimics the Update execution pin
	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UFUNCTION()
	void SetCameraOnAim(float CurrentTime);

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
	virtual void TickAction(float DeltaTime) override;

private:
	FTimeline AimTimeline;

	FOnTimelineFloat ProgressDelegate;
};
