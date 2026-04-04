// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Shoot.generated.h"

/**
 * 
 */
UCLASS()
class CLARITY_API UCAction_Shoot : public UCAction
{
	GENERATED_BODY()
	
public:
	UCAction_Shoot();

	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:

	FName BarrelSocketName;

	FVector CrosshairWorldPosition;

	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bScreenToWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	USoundCue* FiringAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* ImpactEffect;

	bool GetCrosshairWorldProperties(FVector& WorldPosition, FVector& WorldDirection);

	void PlayFireSound(AActor* Instigator);

	void PlayMuzzleFlash(AActor* Instigator, const FTransform SocketTransform);

	void PlayImpactEffect(AActor* Instigator, const FVector& ImpactPoint);

	void PlayWeaponRecoil(AActor* Instigator);
};
