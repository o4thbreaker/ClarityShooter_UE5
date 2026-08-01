// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponData.generated.h"

class ACWeaponBase;
class UAnimMontage;
class UParticleSystem;
class USoundCue;

/**
 * Class that stores data for weapon
 */
UCLASS()
class CLARITY_API UCWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/// \NOTE: recoil is procedural for now
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Animation")
	//UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TSubclassOf<ACWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Effects")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Audio")
	USoundCue* FiringAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float Damage;

	/* fire rate of the weapon. set it in seconds as it is used for shoot cooldown */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	int32 MagazineSize;

	/* how long (in seconds) it takes for victim to return to the original state*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float KnockbackTime;

	/* how hard should victim react to the shot */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float KnockbackForce;

	/* how far should bullet go */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float ShotRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	float RecoilRate;
};
