// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Shoot.generated.h"

class UCWeaponSlotsComponent;
class ACWeaponBase;
struct FCollisionQueryParams;

/**
 * Action that performs a single shot from the currently equipped weapon.
 */
UCLASS()
class CLARITY_API UCAction_Shoot : public UCAction
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(UCActionComponent* NewActionComponent) override;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;

protected:
	/* tags that required for action to start. might be generilzed later*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer RequiredTags;

	float Accuracy;

	bool bWillMiss;

	void PlayFireAnimation(const ACWeaponBase* Weapon);

	void PlayImpactEffect(const ACWeaponBase* Weapon, const FVector& ImpactPoint);

	void PlayWeaponRecoil(const ACWeaponBase* Weapon);
private:
	UPROPERTY()
	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent;

	// set timer to handle remove the fire cooldown timer
	FTimerHandle FireRateCooldownHandle;

	void PerformCrosshairLineTrace(FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params);

	void PerformWeaponLineTrace(const ACWeaponBase* Weapon, FHitResult& WeaponHitResult, const FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params);

	void ProvideDamage(AActor* DamageProvider, AActor* Victim, const ACWeaponBase* Weapon, const FHitResult& WeaponHitResult);

	void DrawDebugLineTrace(const FVector& Start, const FVector& End, const FVector& HitLocation, const FColor& TraceColor, const FColor& HitColor);

	FVector CalculateBulletEndLocation(const ACWeaponBase* Weapon, const FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End) const;

	void ClearFireCooldown();
};
