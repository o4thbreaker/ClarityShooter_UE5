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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UCWeaponData* WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	int32 CurrentAmmoCount;

public:
	FOnAmmoChanged OnAmmoChanged;

	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return WeaponMesh; };

	FORCEINLINE int32 GetCurrentAmmoCount() const { return CurrentAmmoCount; };

	FORCEINLINE UCWeaponData* GetWeaponData() const { return WeaponData; };

	FORCEINLINE void SetCurrentAmmoCount(int32 NewAmmoCount) { CurrentAmmoCount = NewAmmoCount; };

	FORCEINLINE bool CanFire() const;

	bool TryConsumeAmmo();

	virtual void Reload();
};
