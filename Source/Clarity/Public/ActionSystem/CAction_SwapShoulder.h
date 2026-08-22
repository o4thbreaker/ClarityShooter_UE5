// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "Components/TimelineComponent.h"
#include "CAction_SwapShoulder.generated.h"

class USpringArmComponent;

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_SwapShoulder : public UCAction
{
	GENERATED_BODY()
	
public:
	// the Curve is used to control the timeline track
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* TimelineCurve;

	UCAction_SwapShoulder();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* OwnerCameraBoom;

	float InitialSocketOffsetY;

	// Mimics the Update execution pin
	UFUNCTION()
	void HandleTimelineProgress(float Value);

	// Mimics the Finished execution pin
	UFUNCTION()
	void HandleTimelineFinished();

	virtual void Initialize(UCActionComponent* NewActionComponent) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void TickAction(float DeltaTime) override;

private:
	FTimeline SwapTimeline;

	FOnTimelineFloat ProgressDelegate;
	FOnTimelineEvent FinishedDelegate;
};
