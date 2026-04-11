// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponBase.h"

ACWeaponBase::ACWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

}

void ACWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensureAlways(WeaponData))
	{
		CurrentAmmoCount = WeaponData->MagazineSize;
	}
}

bool ACWeaponBase::CanFire() const
{
	return CurrentAmmoCount > 0;
}

bool ACWeaponBase::TryConsumeAmmo()
{
	if (!CanFire()) return false;

	CurrentAmmoCount--;
	OnAmmoChanged.Broadcast(CurrentAmmoCount);

	return true;
}

void ACWeaponBase::Reload()
{
	// sets the full magazine by default
	CurrentAmmoCount = WeaponData->MagazineSize;
	OnAmmoChanged.Broadcast(CurrentAmmoCount);
}

