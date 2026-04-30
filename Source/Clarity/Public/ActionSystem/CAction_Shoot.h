// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Shoot.generated.h"

class UCWeaponSlotsComponent;
class ACWeaponBase;

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
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;

protected:

	FName BarrelSocketName;

	FVector CrosshairWorldPosition;

	FVector CrosshairWorldDirection;

	/* is crosshair translated successfully */
	bool bScreenToWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	USoundCue* FiringAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shoot")
	UParticleSystem* ImpactEffect;

	bool GetFireOriginAndDirection(AActor* Instigator, FVector& OutOrigin, FVector& OutDirection);

	bool GetCrosshairWorldProperties(FVector& WorldPosition, FVector& WorldDirection);

	void PlayFireSound(AActor* Instigator);

	void PlayMuzzleFlash(AActor* Instigator, const FTransform SocketTransform);

	void PlayImpactEffect(AActor* Instigator, const FVector& ImpactPoint);

	void PlayWeaponRecoil(AActor* Instigator);

private:
	UPROPERTY()
	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent;

	UPROPERTY()
	ACWeaponBase* Weapon;
};
