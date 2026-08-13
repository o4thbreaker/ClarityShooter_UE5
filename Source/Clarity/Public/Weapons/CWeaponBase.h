// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponData.h"
#include "CWeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, AmmoValue);

UCLASS()
class CLARITY_API ACWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeaponBase();

	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged OnAmmoChanged;

	/* maybe should be in Weapon Data, but it is the same for all the weapons */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FName BarrelSocketName;

	bool TryConsumeAmmo();

	virtual void Reload();

	FORCEINLINE bool CanFire() const;

	FORCEINLINE void SetCurrentAmmoCount(int32 NewAmmoCount) { CurrentAmmoCount = NewAmmoCount; };
	FORCEINLINE int32 GetCurrentAmmoCount() const { return CurrentAmmoCount; };

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return WeaponMesh; };

	FORCEINLINE UCWeaponData* GetWeaponData() const { return WeaponData; };
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UCWeaponData* WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	int32 CurrentAmmoCount;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

};
