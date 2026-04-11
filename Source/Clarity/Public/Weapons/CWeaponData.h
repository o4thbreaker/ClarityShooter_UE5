// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponData.generated.h"

class ACWeaponBase;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class CLARITY_API UCWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TSubclassOf<ACWeaponBase> WeaponClass;

	/// \NOTE: is this neccessary? or the fire is procedural?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float Damage;

	/* fire rate of the weapon. set it in seconds as it is used for shoot cooldown */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
	int32 MagazineSize;

};
