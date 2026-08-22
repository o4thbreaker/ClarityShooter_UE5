// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/CAction.h"
#include "CAction_Shoot.generated.h"

class UCWeaponSlotsComponent;
class ACWeaponBase;
struct FCollisionQueryParams;

/**
 *
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

	void PlayFireAnimation(AActor* Instigator, const ACWeaponBase* Weapon);

	void PlayImpactEffect(AActor* Instigator, const ACWeaponBase* Weapon, const FVector& ImpactPoint);

	void PlayWeaponRecoil(AActor* Instigator, const ACWeaponBase* Weapon);

	/// \NOTE: DEPRECATED. IT IS NOW DONE VIA ANIMATION
	/*
	void PlayFireSound(AActor* Instigator, ACWeaponBase* Weapon);

	void PlayMuzzleFlash(AActor* Instigator, ACWeaponBase* Weapon, const FTransform& SocketTransform);
	*/
private:
	UPROPERTY()
	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent;

	// set timer to handle remove the fire cooldown timer
	FTimerHandle FireRateCooldownHandle;

	void PerformCrosshairLineTrace(FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params);

	void PerformWeaponLineTrace(ACWeaponBase* Weapon, FHitResult& WeaponHitResult, const FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End, const FCollisionQueryParams& Params);

	void ProvideDamage(AActor* Instigator, AActor* Victim, ACWeaponBase* Weapon, const FHitResult& WeaponHitResult);

	void DrawDebugLineTrace(const FVector& Start, const FVector& End, const FVector& HitLocation, const FColor& TraceColor, const FColor& HitColor);

	FVector CalculateBulletEndLocation(ACWeaponBase* Weapon, const FHitResult& CrosshairHitResult, const FVector& Start, const FVector& End) const;
};
