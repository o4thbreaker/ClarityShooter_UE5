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
	virtual void Initialize(UCActionComponent* NewActionComponent) override;

	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual bool CanStartAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	FGameplayTagContainer RequiredTags;

	void PlayFireSound(AActor* Instigator, ACWeaponBase* Weapon);

	void PlayMuzzleFlash(AActor* Instigator, ACWeaponBase* Weapon, const FTransform SocketTransform);

	void PlayImpactEffect(AActor* Instigator, ACWeaponBase* Weapon, const FVector& ImpactPoint);

	void PlayWeaponRecoil(AActor* Instigator);

private:
	UPROPERTY()
	UCWeaponSlotsComponent* OwnerWeaponSlotsComponent;

	// set timer to handle remove the fire cooldown timer
	FTimerHandle FireRateCooldownHandle;
};
