// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"

ACWeaponBase::ACWeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	BarrelSocketName = FName(TEXT("MuzzleFlash"));
}

void ACWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetMesh()->DoesSocketExist(BarrelSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find Barrel Socket for %s. Check the name of the socket and make sure it is correct."), *GetNameSafe(this));
	}
}

void ACWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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

